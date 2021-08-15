// clang-format off
// Generated file (from: resize_nearest_neighbor.mod.py). Do not edit
void CreateModel_shape_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type1);
  auto param = model->addOperand(&type3);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t param_init[] = {1};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {1};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in, param, param1, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type1);
  auto param = model->addOperand(&type3);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t param_init[] = {1};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {1};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in, param, param1, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.25f, 128);
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.25f, 128);
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type19);
  auto param = model->addOperand(&type3);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param_init[] = {1};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {1};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in, param, param1, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type21(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type22(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type21);
  auto param = model->addOperand(&type3);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type22);
  // Phase 2, operations
  static int32_t param_init[] = {1};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {1};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in, param, param1, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type23);
  auto param = model->addOperand(&type3);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t param_init[] = {1};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {1};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in, param, param1, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type23);
  auto param = model->addOperand(&type3);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t param_init[] = {1};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {1};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in, param, param1, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.25f, 128);
  OperandType type24(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 2}, 0.25f, 128);
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type24);
  auto param = model->addOperand(&type3);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param_init[] = {1};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {1};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in, param, param1, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type22(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type25(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type25);
  auto param = model->addOperand(&type3);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type22);
  // Phase 2, operations
  static int32_t param_init[] = {1};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {1};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in, param, param1, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type1);
  auto param = model->addOperand(&type3);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param_init[] = {1};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {1};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in, param, param1, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
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
  auto in = model->addOperand(&type1);
  auto param = model->addOperand(&type3);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param_init[] = {1};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {1};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in, param, param1, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.25f, 128);
  OperandType type27(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 128);
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type19);
  auto param = model->addOperand(&type3);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param_init[] = {1};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {1};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in, param, param1, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type21(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type21);
  auto param = model->addOperand(&type3);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param_init[] = {1};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {1};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in, param, param1, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type23);
  auto param = model->addOperand(&type3);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param_init[] = {1};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {1};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in, param, param1, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type23);
  auto param = model->addOperand(&type3);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param_init[] = {1};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {1};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in, param, param1, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type24(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 2}, 0.25f, 128);
  OperandType type27(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 128);
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type24);
  auto param = model->addOperand(&type3);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param_init[] = {1};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {1};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in, param, param1, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type25(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type25);
  auto param = model->addOperand(&type3);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param_init[] = {1};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {1};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in, param, param1, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type1);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type2);
  // Phase 2, operations
  static float param2_init[] = {0.5f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in, param2, param3, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type1);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type2);
  // Phase 2, operations
  static float param2_init[] = {0.5f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in, param2, param3, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.25f, 128);
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.25f, 128);
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type19);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type20);
  // Phase 2, operations
  static float param2_init[] = {0.5f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in, param2, param3, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type21(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type22(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type29(Type::FLOAT16, {});
  // Phase 1, operands
  auto in = model->addOperand(&type21);
  auto param2 = model->addOperand(&type29);
  auto param3 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type22);
  // Phase 2, operations
  static _Float16 param2_init[] = {0.5f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in, param2, param3, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type23);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type2);
  // Phase 2, operations
  static float param2_init[] = {0.5f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in, param2, param3, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type23);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type2);
  // Phase 2, operations
  static float param2_init[] = {0.5f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in, param2, param3, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.25f, 128);
  OperandType type24(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 2}, 0.25f, 128);
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type24);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type20);
  // Phase 2, operations
  static float param2_init[] = {0.5f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in, param2, param3, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type22(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type25(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  OperandType type29(Type::FLOAT16, {});
  // Phase 1, operands
  auto in = model->addOperand(&type25);
  auto param2 = model->addOperand(&type29);
  auto param3 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type22);
  // Phase 2, operations
  static _Float16 param2_init[] = {0.5f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in, param2, param3, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type1);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type26);
  // Phase 2, operations
  static float param2_init[] = {0.5f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in, param2, param3, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
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
  auto in = model->addOperand(&type1);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type26);
  // Phase 2, operations
  static float param2_init[] = {0.5f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in, param2, param3, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.25f, 128);
  OperandType type27(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 128);
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type19);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type27);
  // Phase 2, operations
  static float param2_init[] = {0.5f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in, param2, param3, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type21(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type29(Type::FLOAT16, {});
  // Phase 1, operands
  auto in = model->addOperand(&type21);
  auto param2 = model->addOperand(&type29);
  auto param3 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type28);
  // Phase 2, operations
  static _Float16 param2_init[] = {0.5f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in, param2, param3, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type23);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type26);
  // Phase 2, operations
  static float param2_init[] = {0.5f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in, param2, param3, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type23);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type26);
  // Phase 2, operations
  static float param2_init[] = {0.5f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in, param2, param3, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type24(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 2}, 0.25f, 128);
  OperandType type27(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 128);
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type24);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type27);
  // Phase 2, operations
  static float param2_init[] = {0.5f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in, param2, param3, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type25(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type29(Type::FLOAT16, {});
  // Phase 1, operands
  auto in = model->addOperand(&type25);
  auto param2 = model->addOperand(&type29);
  auto param3 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type28);
  // Phase 2, operations
  static _Float16 param2_init[] = {0.5f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in, param2, param3, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  // Phase 1, operands
  auto in1 = model->addOperand(&type1);
  auto param4 = model->addOperand(&type3);
  auto param5 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type5);
  // Phase 2, operations
  static int32_t param4_init[] = {3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in1, param4, param5, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  // Phase 1, operands
  auto in1 = model->addOperand(&type1);
  auto param4 = model->addOperand(&type3);
  auto param5 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type5);
  // Phase 2, operations
  static int32_t param4_init[] = {3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in1, param4, param5, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type30(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.25f, 0);
  OperandType type31(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.25f, 0);
  // Phase 1, operands
  auto in1 = model->addOperand(&type30);
  auto param4 = model->addOperand(&type3);
  auto param5 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type31);
  // Phase 2, operations
  static int32_t param4_init[] = {3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in1, param4, param5, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type21(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type3(Type::INT32, {});
  OperandType type32(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  // Phase 1, operands
  auto in1 = model->addOperand(&type21);
  auto param4 = model->addOperand(&type3);
  auto param5 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type32);
  // Phase 2, operations
  static int32_t param4_init[] = {3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in1, param4, param5, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type3(Type::INT32, {});
  OperandType type33(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  // Phase 1, operands
  auto in1 = model->addOperand(&type23);
  auto param4 = model->addOperand(&type3);
  auto param5 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type33);
  // Phase 2, operations
  static int32_t param4_init[] = {3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in1, param4, param5, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type3(Type::INT32, {});
  OperandType type33(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  // Phase 1, operands
  auto in1 = model->addOperand(&type23);
  auto param4 = model->addOperand(&type3);
  auto param5 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type33);
  // Phase 2, operations
  static int32_t param4_init[] = {3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in1, param4, param5, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type34(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 2}, 0.25f, 0);
  OperandType type35(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 3}, 0.25f, 0);
  // Phase 1, operands
  auto in1 = model->addOperand(&type34);
  auto param4 = model->addOperand(&type3);
  auto param5 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type35);
  // Phase 2, operations
  static int32_t param4_init[] = {3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in1, param4, param5, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type25(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  OperandType type3(Type::INT32, {});
  OperandType type36(Type::TENSOR_FLOAT16, {1, 1, 3, 3});
  // Phase 1, operands
  auto in1 = model->addOperand(&type25);
  auto param4 = model->addOperand(&type3);
  auto param5 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type36);
  // Phase 2, operations
  static int32_t param4_init[] = {3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in1, param4, param5, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in1 = model->addOperand(&type1);
  auto param4 = model->addOperand(&type3);
  auto param5 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param4_init[] = {3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in1, param4, param5, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in1 = model->addOperand(&type1);
  auto param4 = model->addOperand(&type3);
  auto param5 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param4_init[] = {3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in1, param4, param5, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type30(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.25f, 0);
  OperandType type37(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 0);
  // Phase 1, operands
  auto in1 = model->addOperand(&type30);
  auto param4 = model->addOperand(&type3);
  auto param5 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type37);
  // Phase 2, operations
  static int32_t param4_init[] = {3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in1, param4, param5, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type21(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in1 = model->addOperand(&type21);
  auto param4 = model->addOperand(&type3);
  auto param5 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param4_init[] = {3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in1, param4, param5, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in1 = model->addOperand(&type23);
  auto param4 = model->addOperand(&type3);
  auto param5 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param4_init[] = {3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in1, param4, param5, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in1 = model->addOperand(&type23);
  auto param4 = model->addOperand(&type3);
  auto param5 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param4_init[] = {3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in1, param4, param5, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type34(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 2}, 0.25f, 0);
  OperandType type37(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 0);
  // Phase 1, operands
  auto in1 = model->addOperand(&type34);
  auto param4 = model->addOperand(&type3);
  auto param5 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type37);
  // Phase 2, operations
  static int32_t param4_init[] = {3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in1, param4, param5, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type25(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in1 = model->addOperand(&type25);
  auto param4 = model->addOperand(&type3);
  auto param5 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param4_init[] = {3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in1, param4, param5, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type4(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  // Phase 1, operands
  auto in1 = model->addOperand(&type1);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type5);
  // Phase 2, operations
  static float param6_init[] = {1.5f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {1.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in1, param6, param7, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type4(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  // Phase 1, operands
  auto in1 = model->addOperand(&type1);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type5);
  // Phase 2, operations
  static float param6_init[] = {1.5f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {1.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in1, param6, param7, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type30(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.25f, 0);
  OperandType type31(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.25f, 0);
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in1 = model->addOperand(&type30);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type31);
  // Phase 2, operations
  static float param6_init[] = {1.5f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {1.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in1, param6, param7, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type21(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type29(Type::FLOAT16, {});
  OperandType type32(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  // Phase 1, operands
  auto in1 = model->addOperand(&type21);
  auto param6 = model->addOperand(&type29);
  auto param7 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type32);
  // Phase 2, operations
  static _Float16 param6_init[] = {1.5f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {1.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in1, param6, param7, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type33(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in1 = model->addOperand(&type23);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type33);
  // Phase 2, operations
  static float param6_init[] = {1.5f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {1.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in1, param6, param7, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type33(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in1 = model->addOperand(&type23);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type33);
  // Phase 2, operations
  static float param6_init[] = {1.5f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {1.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in1, param6, param7, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type34(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 2}, 0.25f, 0);
  OperandType type35(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 3}, 0.25f, 0);
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in1 = model->addOperand(&type34);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type35);
  // Phase 2, operations
  static float param6_init[] = {1.5f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {1.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in1, param6, param7, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type25(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  OperandType type29(Type::FLOAT16, {});
  OperandType type36(Type::TENSOR_FLOAT16, {1, 1, 3, 3});
  // Phase 1, operands
  auto in1 = model->addOperand(&type25);
  auto param6 = model->addOperand(&type29);
  auto param7 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type36);
  // Phase 2, operations
  static _Float16 param6_init[] = {1.5f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {1.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in1, param6, param7, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in1 = model->addOperand(&type1);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type26);
  // Phase 2, operations
  static float param6_init[] = {1.5f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {1.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in1, param6, param7, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in1 = model->addOperand(&type1);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type26);
  // Phase 2, operations
  static float param6_init[] = {1.5f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {1.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in1, param6, param7, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type30(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.25f, 0);
  OperandType type37(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 0);
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in1 = model->addOperand(&type30);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type37);
  // Phase 2, operations
  static float param6_init[] = {1.5f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {1.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in1, param6, param7, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type21(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type29(Type::FLOAT16, {});
  // Phase 1, operands
  auto in1 = model->addOperand(&type21);
  auto param6 = model->addOperand(&type29);
  auto param7 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type28);
  // Phase 2, operations
  static _Float16 param6_init[] = {1.5f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {1.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in1, param6, param7, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in1 = model->addOperand(&type23);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type26);
  // Phase 2, operations
  static float param6_init[] = {1.5f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {1.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in1, param6, param7, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in1 = model->addOperand(&type23);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type26);
  // Phase 2, operations
  static float param6_init[] = {1.5f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {1.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in1, param6, param7, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type34(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 2}, 0.25f, 0);
  OperandType type37(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 0);
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in1 = model->addOperand(&type34);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type37);
  // Phase 2, operations
  static float param6_init[] = {1.5f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {1.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in1, param6, param7, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type25(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type29(Type::FLOAT16, {});
  // Phase 1, operands
  auto in1 = model->addOperand(&type25);
  auto param6 = model->addOperand(&type29);
  auto param7 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type28);
  // Phase 2, operations
  static _Float16 param6_init[] = {1.5f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {1.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in1, param6, param7, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  // Phase 1, operands
  auto in2 = model->addOperand(&type5);
  auto param8 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type1);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {2};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in2, param8, param9, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_relaxed_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  // Phase 1, operands
  auto in2 = model->addOperand(&type5);
  auto param8 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type1);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {2};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in2, param8, param9, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2},
    {out2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_relaxed_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_quant8_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type38(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.25f, 100);
  OperandType type39(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.25f, 100);
  // Phase 1, operands
  auto in2 = model->addOperand(&type38);
  auto param8 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type39);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {2};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in2, param8, param9, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_quant8_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_float16_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type21(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type3(Type::INT32, {});
  OperandType type32(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  // Phase 1, operands
  auto in2 = model->addOperand(&type32);
  auto param8 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {2};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in2, param8, param9, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type3(Type::INT32, {});
  OperandType type33(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  // Phase 1, operands
  auto in2 = model->addOperand(&type33);
  auto param8 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {2};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in2, param8, param9, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_relaxed_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type3(Type::INT32, {});
  OperandType type33(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  // Phase 1, operands
  auto in2 = model->addOperand(&type33);
  auto param8 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {2};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in2, param8, param9, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2},
    {out2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_relaxed_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_quant8_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type40(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 3}, 0.25f, 100);
  OperandType type41(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 2}, 0.25f, 100);
  // Phase 1, operands
  auto in2 = model->addOperand(&type40);
  auto param8 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type41);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {2};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in2, param8, param9, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_quant8_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_float16_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type25(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  OperandType type3(Type::INT32, {});
  OperandType type36(Type::TENSOR_FLOAT16, {1, 1, 3, 3});
  // Phase 1, operands
  auto in2 = model->addOperand(&type36);
  auto param8 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {2};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in2, param8, param9, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  // Phase 1, operands
  auto in2 = model->addOperand(&type5);
  auto param8 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {2};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in2, param8, param9, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_relaxed_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  // Phase 1, operands
  auto in2 = model->addOperand(&type5);
  auto param8 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {2};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in2, param8, param9, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2},
    {out2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_relaxed_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_quant8_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type38(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.25f, 100);
  OperandType type42(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 100);
  // Phase 1, operands
  auto in2 = model->addOperand(&type38);
  auto param8 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type42);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {2};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in2, param8, param9, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_quant8_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_float16_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type32(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  // Phase 1, operands
  auto in2 = model->addOperand(&type32);
  auto param8 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {2};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in2, param8, param9, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type33(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  // Phase 1, operands
  auto in2 = model->addOperand(&type33);
  auto param8 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {2};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in2, param8, param9, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_relaxed_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type33(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  // Phase 1, operands
  auto in2 = model->addOperand(&type33);
  auto param8 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {2};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in2, param8, param9, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2},
    {out2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_relaxed_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_quant8_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type40(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 3}, 0.25f, 100);
  OperandType type42(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 100);
  // Phase 1, operands
  auto in2 = model->addOperand(&type40);
  auto param8 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type42);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {2};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in2, param8, param9, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_quant8_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_float16_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type36(Type::TENSOR_FLOAT16, {1, 1, 3, 3});
  // Phase 1, operands
  auto in2 = model->addOperand(&type36);
  auto param8 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {2};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in2, param8, param9, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type4(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  // Phase 1, operands
  auto in2 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type1);
  // Phase 2, operations
  static float param10_init[] = {0.8f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.8f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in2, param10, param11, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_relaxed_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type4(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  // Phase 1, operands
  auto in2 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type1);
  // Phase 2, operations
  static float param10_init[] = {0.8f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.8f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in2, param10, param11, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2},
    {out2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_relaxed_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_quant8_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type38(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.25f, 100);
  OperandType type39(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.25f, 100);
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in2 = model->addOperand(&type38);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type39);
  // Phase 2, operations
  static float param10_init[] = {0.8f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.8f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in2, param10, param11, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_quant8_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_float16_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type21(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type29(Type::FLOAT16, {});
  OperandType type32(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  // Phase 1, operands
  auto in2 = model->addOperand(&type32);
  auto param10 = model->addOperand(&type29);
  auto param11 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type21);
  // Phase 2, operations
  static _Float16 param10_init[] = {0.800000011920929f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.800000011920929f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in2, param10, param11, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type33(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in2 = model->addOperand(&type33);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type23);
  // Phase 2, operations
  static float param10_init[] = {0.8f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.8f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in2, param10, param11, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_relaxed_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type33(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in2 = model->addOperand(&type33);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type23);
  // Phase 2, operations
  static float param10_init[] = {0.8f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.8f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in2, param10, param11, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2},
    {out2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_relaxed_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_quant8_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type40(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 3}, 0.25f, 100);
  OperandType type41(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 2}, 0.25f, 100);
  // Phase 1, operands
  auto in2 = model->addOperand(&type40);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type41);
  // Phase 2, operations
  static float param10_init[] = {0.8f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.8f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in2, param10, param11, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_quant8_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_float16_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type25(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  OperandType type29(Type::FLOAT16, {});
  OperandType type36(Type::TENSOR_FLOAT16, {1, 1, 3, 3});
  // Phase 1, operands
  auto in2 = model->addOperand(&type36);
  auto param10 = model->addOperand(&type29);
  auto param11 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type25);
  // Phase 2, operations
  static _Float16 param10_init[] = {0.800000011920929f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.800000011920929f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in2, param10, param11, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  // Phase 1, operands
  auto in2 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type26);
  // Phase 2, operations
  static float param10_init[] = {0.8f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.8f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in2, param10, param11, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_relaxed_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  // Phase 1, operands
  auto in2 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type26);
  // Phase 2, operations
  static float param10_init[] = {0.8f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.8f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in2, param10, param11, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2},
    {out2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_relaxed_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_quant8_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type38(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.25f, 100);
  OperandType type4(Type::FLOAT32, {});
  OperandType type42(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 100);
  // Phase 1, operands
  auto in2 = model->addOperand(&type38);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type42);
  // Phase 2, operations
  static float param10_init[] = {0.8f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.8f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in2, param10, param11, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_quant8_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_float16_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type29(Type::FLOAT16, {});
  OperandType type32(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  // Phase 1, operands
  auto in2 = model->addOperand(&type32);
  auto param10 = model->addOperand(&type29);
  auto param11 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type28);
  // Phase 2, operations
  static _Float16 param10_init[] = {0.800000011920929f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.800000011920929f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in2, param10, param11, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type33(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in2 = model->addOperand(&type33);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type26);
  // Phase 2, operations
  static float param10_init[] = {0.8f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.8f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in2, param10, param11, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_relaxed_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type33(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in2 = model->addOperand(&type33);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type26);
  // Phase 2, operations
  static float param10_init[] = {0.8f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.8f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in2, param10, param11, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2},
    {out2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_relaxed_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_quant8_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type40(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 3}, 0.25f, 100);
  OperandType type42(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 100);
  // Phase 1, operands
  auto in2 = model->addOperand(&type40);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type42);
  // Phase 2, operations
  static float param10_init[] = {0.8f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.8f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in2, param10, param11, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_quant8_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_float16_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type29(Type::FLOAT16, {});
  OperandType type36(Type::TENSOR_FLOAT16, {1, 1, 3, 3});
  // Phase 1, operands
  auto in2 = model->addOperand(&type36);
  auto param10 = model->addOperand(&type29);
  auto param11 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type28);
  // Phase 2, operations
  static _Float16 param10_init[] = {0.800000011920929f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.800000011920929f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in2, param10, param11, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::INT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 2, 5, 1});
  // Phase 1, operands
  auto in3 = model->addOperand(&type1);
  auto param12 = model->addOperand(&type3);
  auto param13 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out3 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param12_init[] = {5};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in3, param12, param13, layout}, {out3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in3},
    {out3});
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_relaxed_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::INT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 2, 5, 1});
  // Phase 1, operands
  auto in3 = model->addOperand(&type1);
  auto param12 = model->addOperand(&type3);
  auto param13 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out3 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param12_init[] = {5};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in3, param12, param13, layout}, {out3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in3},
    {out3});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_relaxed_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_quant8_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type39(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.25f, 100);
  OperandType type43(Type::TENSOR_QUANT8_ASYMM, {1, 2, 5, 1}, 0.25f, 100);
  // Phase 1, operands
  auto in3 = model->addOperand(&type39);
  auto param12 = model->addOperand(&type3);
  auto param13 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out3 = model->addOperand(&type43);
  // Phase 2, operations
  static int32_t param12_init[] = {5};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in3, param12, param13, layout}, {out3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in3},
    {out3});
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_quant8_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_float16_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type21(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type3(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT16, {1, 2, 5, 1});
  // Phase 1, operands
  auto in3 = model->addOperand(&type21);
  auto param12 = model->addOperand(&type3);
  auto param13 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out3 = model->addOperand(&type44);
  // Phase 2, operations
  static int32_t param12_init[] = {5};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in3, param12, param13, layout}, {out3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in3},
    {out3});
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_float16_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type3(Type::INT32, {});
  OperandType type45(Type::TENSOR_FLOAT32, {1, 1, 2, 5});
  // Phase 1, operands
  auto in3 = model->addOperand(&type23);
  auto param12 = model->addOperand(&type3);
  auto param13 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out3 = model->addOperand(&type45);
  // Phase 2, operations
  static int32_t param12_init[] = {5};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in3, param12, param13, layout}, {out3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in3},
    {out3});
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_relaxed_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type3(Type::INT32, {});
  OperandType type45(Type::TENSOR_FLOAT32, {1, 1, 2, 5});
  // Phase 1, operands
  auto in3 = model->addOperand(&type23);
  auto param12 = model->addOperand(&type3);
  auto param13 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out3 = model->addOperand(&type45);
  // Phase 2, operations
  static int32_t param12_init[] = {5};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in3, param12, param13, layout}, {out3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in3},
    {out3});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_relaxed_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_quant8_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type41(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 2}, 0.25f, 100);
  OperandType type46(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 5}, 0.25f, 100);
  // Phase 1, operands
  auto in3 = model->addOperand(&type41);
  auto param12 = model->addOperand(&type3);
  auto param13 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out3 = model->addOperand(&type46);
  // Phase 2, operations
  static int32_t param12_init[] = {5};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in3, param12, param13, layout}, {out3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in3},
    {out3});
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_quant8_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_float16_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type25(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  OperandType type3(Type::INT32, {});
  OperandType type47(Type::TENSOR_FLOAT16, {1, 1, 2, 5});
  // Phase 1, operands
  auto in3 = model->addOperand(&type25);
  auto param12 = model->addOperand(&type3);
  auto param13 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out3 = model->addOperand(&type47);
  // Phase 2, operations
  static int32_t param12_init[] = {5};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in3, param12, param13, layout}, {out3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in3},
    {out3});
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_float16_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in3 = model->addOperand(&type1);
  auto param12 = model->addOperand(&type3);
  auto param13 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out3 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param12_init[] = {5};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in3, param12, param13, layout}, {out3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in3},
    {out3});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_relaxed_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in3 = model->addOperand(&type1);
  auto param12 = model->addOperand(&type3);
  auto param13 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out3 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param12_init[] = {5};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in3, param12, param13, layout}, {out3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in3},
    {out3});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_relaxed_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_quant8_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type39(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.25f, 100);
  OperandType type42(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 100);
  // Phase 1, operands
  auto in3 = model->addOperand(&type39);
  auto param12 = model->addOperand(&type3);
  auto param13 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out3 = model->addOperand(&type42);
  // Phase 2, operations
  static int32_t param12_init[] = {5};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in3, param12, param13, layout}, {out3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in3},
    {out3});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_quant8_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_float16_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type21(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in3 = model->addOperand(&type21);
  auto param12 = model->addOperand(&type3);
  auto param13 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out3 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param12_init[] = {5};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in3, param12, param13, layout}, {out3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in3},
    {out3});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_float16_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in3 = model->addOperand(&type23);
  auto param12 = model->addOperand(&type3);
  auto param13 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out3 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param12_init[] = {5};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in3, param12, param13, layout}, {out3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in3},
    {out3});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_relaxed_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in3 = model->addOperand(&type23);
  auto param12 = model->addOperand(&type3);
  auto param13 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out3 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param12_init[] = {5};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in3, param12, param13, layout}, {out3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in3},
    {out3});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_relaxed_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_quant8_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type41(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 2}, 0.25f, 100);
  OperandType type42(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 100);
  // Phase 1, operands
  auto in3 = model->addOperand(&type41);
  auto param12 = model->addOperand(&type3);
  auto param13 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out3 = model->addOperand(&type42);
  // Phase 2, operations
  static int32_t param12_init[] = {5};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in3, param12, param13, layout}, {out3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in3},
    {out3});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_quant8_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_float16_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type25(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in3 = model->addOperand(&type25);
  auto param12 = model->addOperand(&type3);
  auto param13 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out3 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param12_init[] = {5};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in3, param12, param13, layout}, {out3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in3},
    {out3});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_float16_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type4(Type::FLOAT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 2, 5, 1});
  // Phase 1, operands
  auto in3 = model->addOperand(&type1);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out3 = model->addOperand(&type6);
  // Phase 2, operations
  static float param14_init[] = {2.6f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static float param15_init[] = {1.1f};
  model->setOperandValue(param15, param15_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in3, param14, param15, layout}, {out3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in3},
    {out3});
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_relaxed_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type4(Type::FLOAT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 2, 5, 1});
  // Phase 1, operands
  auto in3 = model->addOperand(&type1);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out3 = model->addOperand(&type6);
  // Phase 2, operations
  static float param14_init[] = {2.6f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static float param15_init[] = {1.1f};
  model->setOperandValue(param15, param15_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in3, param14, param15, layout}, {out3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in3},
    {out3});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_relaxed_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_quant8_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type39(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.25f, 100);
  OperandType type4(Type::FLOAT32, {});
  OperandType type43(Type::TENSOR_QUANT8_ASYMM, {1, 2, 5, 1}, 0.25f, 100);
  // Phase 1, operands
  auto in3 = model->addOperand(&type39);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out3 = model->addOperand(&type43);
  // Phase 2, operations
  static float param14_init[] = {2.6f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static float param15_init[] = {1.1f};
  model->setOperandValue(param15, param15_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in3, param14, param15, layout}, {out3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in3},
    {out3});
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_quant8_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_float16_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type21(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type29(Type::FLOAT16, {});
  OperandType type44(Type::TENSOR_FLOAT16, {1, 2, 5, 1});
  // Phase 1, operands
  auto in3 = model->addOperand(&type21);
  auto param14 = model->addOperand(&type29);
  auto param15 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto out3 = model->addOperand(&type44);
  // Phase 2, operations
  static _Float16 param14_init[] = {2.5999999046325684f};
  model->setOperandValue(param14, param14_init, sizeof(_Float16) * 1);
  static _Float16 param15_init[] = {1.100000023841858f};
  model->setOperandValue(param15, param15_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in3, param14, param15, layout}, {out3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in3},
    {out3});
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_float16_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type4(Type::FLOAT32, {});
  OperandType type45(Type::TENSOR_FLOAT32, {1, 1, 2, 5});
  // Phase 1, operands
  auto in3 = model->addOperand(&type23);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out3 = model->addOperand(&type45);
  // Phase 2, operations
  static float param14_init[] = {2.6f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static float param15_init[] = {1.1f};
  model->setOperandValue(param15, param15_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in3, param14, param15, layout}, {out3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in3},
    {out3});
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_relaxed_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type4(Type::FLOAT32, {});
  OperandType type45(Type::TENSOR_FLOAT32, {1, 1, 2, 5});
  // Phase 1, operands
  auto in3 = model->addOperand(&type23);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out3 = model->addOperand(&type45);
  // Phase 2, operations
  static float param14_init[] = {2.6f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static float param15_init[] = {1.1f};
  model->setOperandValue(param15, param15_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in3, param14, param15, layout}, {out3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in3},
    {out3});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_relaxed_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_quant8_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type41(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 2}, 0.25f, 100);
  OperandType type46(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 5}, 0.25f, 100);
  // Phase 1, operands
  auto in3 = model->addOperand(&type41);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out3 = model->addOperand(&type46);
  // Phase 2, operations
  static float param14_init[] = {2.6f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static float param15_init[] = {1.1f};
  model->setOperandValue(param15, param15_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in3, param14, param15, layout}, {out3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in3},
    {out3});
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_quant8_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_float16_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type25(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  OperandType type29(Type::FLOAT16, {});
  OperandType type47(Type::TENSOR_FLOAT16, {1, 1, 2, 5});
  // Phase 1, operands
  auto in3 = model->addOperand(&type25);
  auto param14 = model->addOperand(&type29);
  auto param15 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto out3 = model->addOperand(&type47);
  // Phase 2, operations
  static _Float16 param14_init[] = {2.5999999046325684f};
  model->setOperandValue(param14, param14_init, sizeof(_Float16) * 1);
  static _Float16 param15_init[] = {1.100000023841858f};
  model->setOperandValue(param15, param15_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in3, param14, param15, layout}, {out3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in3},
    {out3});
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_float16_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in3 = model->addOperand(&type1);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out3 = model->addOperand(&type26);
  // Phase 2, operations
  static float param14_init[] = {2.6f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static float param15_init[] = {1.1f};
  model->setOperandValue(param15, param15_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in3, param14, param15, layout}, {out3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in3},
    {out3});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_relaxed_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in3 = model->addOperand(&type1);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out3 = model->addOperand(&type26);
  // Phase 2, operations
  static float param14_init[] = {2.6f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static float param15_init[] = {1.1f};
  model->setOperandValue(param15, param15_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in3, param14, param15, layout}, {out3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in3},
    {out3});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_relaxed_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_quant8_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type39(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.25f, 100);
  OperandType type4(Type::FLOAT32, {});
  OperandType type42(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 100);
  // Phase 1, operands
  auto in3 = model->addOperand(&type39);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out3 = model->addOperand(&type42);
  // Phase 2, operations
  static float param14_init[] = {2.6f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static float param15_init[] = {1.1f};
  model->setOperandValue(param15, param15_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in3, param14, param15, layout}, {out3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in3},
    {out3});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_quant8_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_float16_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type21(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type29(Type::FLOAT16, {});
  // Phase 1, operands
  auto in3 = model->addOperand(&type21);
  auto param14 = model->addOperand(&type29);
  auto param15 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto out3 = model->addOperand(&type28);
  // Phase 2, operations
  static _Float16 param14_init[] = {2.5999999046325684f};
  model->setOperandValue(param14, param14_init, sizeof(_Float16) * 1);
  static _Float16 param15_init[] = {1.100000023841858f};
  model->setOperandValue(param15, param15_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in3, param14, param15, layout}, {out3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in3},
    {out3});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_float16_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in3 = model->addOperand(&type23);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out3 = model->addOperand(&type26);
  // Phase 2, operations
  static float param14_init[] = {2.6f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static float param15_init[] = {1.1f};
  model->setOperandValue(param15, param15_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in3, param14, param15, layout}, {out3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in3},
    {out3});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_relaxed_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in3 = model->addOperand(&type23);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out3 = model->addOperand(&type26);
  // Phase 2, operations
  static float param14_init[] = {2.6f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static float param15_init[] = {1.1f};
  model->setOperandValue(param15, param15_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in3, param14, param15, layout}, {out3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in3},
    {out3});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_relaxed_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_quant8_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type41(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 2}, 0.25f, 100);
  OperandType type42(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 100);
  // Phase 1, operands
  auto in3 = model->addOperand(&type41);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out3 = model->addOperand(&type42);
  // Phase 2, operations
  static float param14_init[] = {2.6f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static float param15_init[] = {1.1f};
  model->setOperandValue(param15, param15_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in3, param14, param15, layout}, {out3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in3},
    {out3});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_quant8_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_float16_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type25(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type29(Type::FLOAT16, {});
  // Phase 1, operands
  auto in3 = model->addOperand(&type25);
  auto param14 = model->addOperand(&type29);
  auto param15 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto out3 = model->addOperand(&type28);
  // Phase 2, operations
  static _Float16 param14_init[] = {2.5999999046325684f};
  model->setOperandValue(param14, param14_init, sizeof(_Float16) * 1);
  static _Float16 param15_init[] = {1.100000023841858f};
  model->setOperandValue(param15, param15_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in3, param14, param15, layout}, {out3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in3},
    {out3});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_float16_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 4, 4, 1});
  // Phase 1, operands
  auto in4 = model->addOperand(&type7);
  auto param16 = model->addOperand(&type3);
  auto param17 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out4 = model->addOperand(&type5);
  // Phase 2, operations
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in4, param16, param17, layout}, {out4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in4},
    {out4});
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_relaxed_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 4, 4, 1});
  // Phase 1, operands
  auto in4 = model->addOperand(&type7);
  auto param16 = model->addOperand(&type3);
  auto param17 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out4 = model->addOperand(&type5);
  // Phase 2, operations
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in4, param16, param17, layout}, {out4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in4},
    {out4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_relaxed_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_quant8_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type38(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.25f, 100);
  OperandType type48(Type::TENSOR_QUANT8_ASYMM, {1, 4, 4, 1}, 0.25f, 100);
  // Phase 1, operands
  auto in4 = model->addOperand(&type48);
  auto param16 = model->addOperand(&type3);
  auto param17 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out4 = model->addOperand(&type38);
  // Phase 2, operations
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in4, param16, param17, layout}, {out4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in4},
    {out4});
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_quant8_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_float16_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type32(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type49(Type::TENSOR_FLOAT16, {1, 4, 4, 1});
  // Phase 1, operands
  auto in4 = model->addOperand(&type49);
  auto param16 = model->addOperand(&type3);
  auto param17 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out4 = model->addOperand(&type32);
  // Phase 2, operations
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in4, param16, param17, layout}, {out4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in4},
    {out4});
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_float16_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type33(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type50(Type::TENSOR_FLOAT32, {1, 1, 4, 4});
  // Phase 1, operands
  auto in4 = model->addOperand(&type50);
  auto param16 = model->addOperand(&type3);
  auto param17 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out4 = model->addOperand(&type33);
  // Phase 2, operations
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in4, param16, param17, layout}, {out4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in4},
    {out4});
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_relaxed_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type33(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type50(Type::TENSOR_FLOAT32, {1, 1, 4, 4});
  // Phase 1, operands
  auto in4 = model->addOperand(&type50);
  auto param16 = model->addOperand(&type3);
  auto param17 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out4 = model->addOperand(&type33);
  // Phase 2, operations
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in4, param16, param17, layout}, {out4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in4},
    {out4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_relaxed_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_quant8_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type40(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 3}, 0.25f, 100);
  OperandType type51(Type::TENSOR_QUANT8_ASYMM, {1, 1, 4, 4}, 0.25f, 100);
  // Phase 1, operands
  auto in4 = model->addOperand(&type51);
  auto param16 = model->addOperand(&type3);
  auto param17 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out4 = model->addOperand(&type40);
  // Phase 2, operations
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in4, param16, param17, layout}, {out4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in4},
    {out4});
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_quant8_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_float16_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type36(Type::TENSOR_FLOAT16, {1, 1, 3, 3});
  OperandType type52(Type::TENSOR_FLOAT16, {1, 1, 4, 4});
  // Phase 1, operands
  auto in4 = model->addOperand(&type52);
  auto param16 = model->addOperand(&type3);
  auto param17 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out4 = model->addOperand(&type36);
  // Phase 2, operations
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in4, param16, param17, layout}, {out4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in4},
    {out4});
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_float16_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 4, 4, 1});
  // Phase 1, operands
  auto in4 = model->addOperand(&type7);
  auto param16 = model->addOperand(&type3);
  auto param17 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out4 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in4, param16, param17, layout}, {out4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in4},
    {out4});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_relaxed_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 4, 4, 1});
  // Phase 1, operands
  auto in4 = model->addOperand(&type7);
  auto param16 = model->addOperand(&type3);
  auto param17 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out4 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in4, param16, param17, layout}, {out4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in4},
    {out4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_relaxed_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_quant8_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type42(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 100);
  OperandType type48(Type::TENSOR_QUANT8_ASYMM, {1, 4, 4, 1}, 0.25f, 100);
  // Phase 1, operands
  auto in4 = model->addOperand(&type48);
  auto param16 = model->addOperand(&type3);
  auto param17 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out4 = model->addOperand(&type42);
  // Phase 2, operations
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in4, param16, param17, layout}, {out4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in4},
    {out4});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_quant8_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_float16_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type49(Type::TENSOR_FLOAT16, {1, 4, 4, 1});
  // Phase 1, operands
  auto in4 = model->addOperand(&type49);
  auto param16 = model->addOperand(&type3);
  auto param17 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out4 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in4, param16, param17, layout}, {out4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in4},
    {out4});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_float16_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type50(Type::TENSOR_FLOAT32, {1, 1, 4, 4});
  // Phase 1, operands
  auto in4 = model->addOperand(&type50);
  auto param16 = model->addOperand(&type3);
  auto param17 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out4 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in4, param16, param17, layout}, {out4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in4},
    {out4});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_relaxed_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type50(Type::TENSOR_FLOAT32, {1, 1, 4, 4});
  // Phase 1, operands
  auto in4 = model->addOperand(&type50);
  auto param16 = model->addOperand(&type3);
  auto param17 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out4 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in4, param16, param17, layout}, {out4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in4},
    {out4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_relaxed_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_quant8_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type42(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 100);
  OperandType type51(Type::TENSOR_QUANT8_ASYMM, {1, 1, 4, 4}, 0.25f, 100);
  // Phase 1, operands
  auto in4 = model->addOperand(&type51);
  auto param16 = model->addOperand(&type3);
  auto param17 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out4 = model->addOperand(&type42);
  // Phase 2, operations
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in4, param16, param17, layout}, {out4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in4},
    {out4});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_quant8_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_float16_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type52(Type::TENSOR_FLOAT16, {1, 1, 4, 4});
  // Phase 1, operands
  auto in4 = model->addOperand(&type52);
  auto param16 = model->addOperand(&type3);
  auto param17 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out4 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in4, param16, param17, layout}, {out4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in4},
    {out4});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_float16_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 4, 4, 1});
  // Phase 1, operands
  auto in4 = model->addOperand(&type7);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out4 = model->addOperand(&type5);
  // Phase 2, operations
  static float param18_init[] = {0.9f};
  model->setOperandValue(param18, param18_init, sizeof(float) * 1);
  static float param19_init[] = {0.9f};
  model->setOperandValue(param19, param19_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in4, param18, param19, layout}, {out4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in4},
    {out4});
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_relaxed_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 4, 4, 1});
  // Phase 1, operands
  auto in4 = model->addOperand(&type7);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out4 = model->addOperand(&type5);
  // Phase 2, operations
  static float param18_init[] = {0.9f};
  model->setOperandValue(param18, param18_init, sizeof(float) * 1);
  static float param19_init[] = {0.9f};
  model->setOperandValue(param19, param19_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in4, param18, param19, layout}, {out4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in4},
    {out4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_relaxed_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_quant8_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type38(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.25f, 100);
  OperandType type4(Type::FLOAT32, {});
  OperandType type48(Type::TENSOR_QUANT8_ASYMM, {1, 4, 4, 1}, 0.25f, 100);
  // Phase 1, operands
  auto in4 = model->addOperand(&type48);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out4 = model->addOperand(&type38);
  // Phase 2, operations
  static float param18_init[] = {0.9f};
  model->setOperandValue(param18, param18_init, sizeof(float) * 1);
  static float param19_init[] = {0.9f};
  model->setOperandValue(param19, param19_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in4, param18, param19, layout}, {out4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in4},
    {out4});
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_quant8_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_float16_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type29(Type::FLOAT16, {});
  OperandType type32(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type49(Type::TENSOR_FLOAT16, {1, 4, 4, 1});
  // Phase 1, operands
  auto in4 = model->addOperand(&type49);
  auto param18 = model->addOperand(&type29);
  auto param19 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto out4 = model->addOperand(&type32);
  // Phase 2, operations
  static _Float16 param18_init[] = {0.8999999761581421f};
  model->setOperandValue(param18, param18_init, sizeof(_Float16) * 1);
  static _Float16 param19_init[] = {0.8999999761581421f};
  model->setOperandValue(param19, param19_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in4, param18, param19, layout}, {out4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in4},
    {out4});
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_float16_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type33(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type4(Type::FLOAT32, {});
  OperandType type50(Type::TENSOR_FLOAT32, {1, 1, 4, 4});
  // Phase 1, operands
  auto in4 = model->addOperand(&type50);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out4 = model->addOperand(&type33);
  // Phase 2, operations
  static float param18_init[] = {0.9f};
  model->setOperandValue(param18, param18_init, sizeof(float) * 1);
  static float param19_init[] = {0.9f};
  model->setOperandValue(param19, param19_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in4, param18, param19, layout}, {out4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in4},
    {out4});
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_relaxed_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type33(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type4(Type::FLOAT32, {});
  OperandType type50(Type::TENSOR_FLOAT32, {1, 1, 4, 4});
  // Phase 1, operands
  auto in4 = model->addOperand(&type50);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out4 = model->addOperand(&type33);
  // Phase 2, operations
  static float param18_init[] = {0.9f};
  model->setOperandValue(param18, param18_init, sizeof(float) * 1);
  static float param19_init[] = {0.9f};
  model->setOperandValue(param19, param19_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in4, param18, param19, layout}, {out4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in4},
    {out4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_relaxed_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_quant8_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type40(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 3}, 0.25f, 100);
  OperandType type51(Type::TENSOR_QUANT8_ASYMM, {1, 1, 4, 4}, 0.25f, 100);
  // Phase 1, operands
  auto in4 = model->addOperand(&type51);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out4 = model->addOperand(&type40);
  // Phase 2, operations
  static float param18_init[] = {0.9f};
  model->setOperandValue(param18, param18_init, sizeof(float) * 1);
  static float param19_init[] = {0.9f};
  model->setOperandValue(param19, param19_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in4, param18, param19, layout}, {out4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in4},
    {out4});
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_quant8_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_float16_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type29(Type::FLOAT16, {});
  OperandType type36(Type::TENSOR_FLOAT16, {1, 1, 3, 3});
  OperandType type52(Type::TENSOR_FLOAT16, {1, 1, 4, 4});
  // Phase 1, operands
  auto in4 = model->addOperand(&type52);
  auto param18 = model->addOperand(&type29);
  auto param19 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto out4 = model->addOperand(&type36);
  // Phase 2, operations
  static _Float16 param18_init[] = {0.8999999761581421f};
  model->setOperandValue(param18, param18_init, sizeof(_Float16) * 1);
  static _Float16 param19_init[] = {0.8999999761581421f};
  model->setOperandValue(param19, param19_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in4, param18, param19, layout}, {out4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in4},
    {out4});
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_float16_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 4, 4, 1});
  // Phase 1, operands
  auto in4 = model->addOperand(&type7);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out4 = model->addOperand(&type26);
  // Phase 2, operations
  static float param18_init[] = {0.9f};
  model->setOperandValue(param18, param18_init, sizeof(float) * 1);
  static float param19_init[] = {0.9f};
  model->setOperandValue(param19, param19_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in4, param18, param19, layout}, {out4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in4},
    {out4});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_relaxed_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 4, 4, 1});
  // Phase 1, operands
  auto in4 = model->addOperand(&type7);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out4 = model->addOperand(&type26);
  // Phase 2, operations
  static float param18_init[] = {0.9f};
  model->setOperandValue(param18, param18_init, sizeof(float) * 1);
  static float param19_init[] = {0.9f};
  model->setOperandValue(param19, param19_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in4, param18, param19, layout}, {out4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in4},
    {out4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_relaxed_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_quant8_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type42(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 100);
  OperandType type48(Type::TENSOR_QUANT8_ASYMM, {1, 4, 4, 1}, 0.25f, 100);
  // Phase 1, operands
  auto in4 = model->addOperand(&type48);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out4 = model->addOperand(&type42);
  // Phase 2, operations
  static float param18_init[] = {0.9f};
  model->setOperandValue(param18, param18_init, sizeof(float) * 1);
  static float param19_init[] = {0.9f};
  model->setOperandValue(param19, param19_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in4, param18, param19, layout}, {out4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in4},
    {out4});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_quant8_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_float16_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type29(Type::FLOAT16, {});
  OperandType type49(Type::TENSOR_FLOAT16, {1, 4, 4, 1});
  // Phase 1, operands
  auto in4 = model->addOperand(&type49);
  auto param18 = model->addOperand(&type29);
  auto param19 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto out4 = model->addOperand(&type28);
  // Phase 2, operations
  static _Float16 param18_init[] = {0.8999999761581421f};
  model->setOperandValue(param18, param18_init, sizeof(_Float16) * 1);
  static _Float16 param19_init[] = {0.8999999761581421f};
  model->setOperandValue(param19, param19_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in4, param18, param19, layout}, {out4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in4},
    {out4});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_float16_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  OperandType type50(Type::TENSOR_FLOAT32, {1, 1, 4, 4});
  // Phase 1, operands
  auto in4 = model->addOperand(&type50);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out4 = model->addOperand(&type26);
  // Phase 2, operations
  static float param18_init[] = {0.9f};
  model->setOperandValue(param18, param18_init, sizeof(float) * 1);
  static float param19_init[] = {0.9f};
  model->setOperandValue(param19, param19_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in4, param18, param19, layout}, {out4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in4},
    {out4});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_relaxed_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  OperandType type50(Type::TENSOR_FLOAT32, {1, 1, 4, 4});
  // Phase 1, operands
  auto in4 = model->addOperand(&type50);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out4 = model->addOperand(&type26);
  // Phase 2, operations
  static float param18_init[] = {0.9f};
  model->setOperandValue(param18, param18_init, sizeof(float) * 1);
  static float param19_init[] = {0.9f};
  model->setOperandValue(param19, param19_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in4, param18, param19, layout}, {out4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in4},
    {out4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_relaxed_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_quant8_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type42(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 100);
  OperandType type51(Type::TENSOR_QUANT8_ASYMM, {1, 1, 4, 4}, 0.25f, 100);
  // Phase 1, operands
  auto in4 = model->addOperand(&type51);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out4 = model->addOperand(&type42);
  // Phase 2, operations
  static float param18_init[] = {0.9f};
  model->setOperandValue(param18, param18_init, sizeof(float) * 1);
  static float param19_init[] = {0.9f};
  model->setOperandValue(param19, param19_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in4, param18, param19, layout}, {out4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in4},
    {out4});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_quant8_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_float16_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type29(Type::FLOAT16, {});
  OperandType type52(Type::TENSOR_FLOAT16, {1, 1, 4, 4});
  // Phase 1, operands
  auto in4 = model->addOperand(&type52);
  auto param18 = model->addOperand(&type29);
  auto param19 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto out4 = model->addOperand(&type28);
  // Phase 2, operations
  static _Float16 param18_init[] = {0.8999999761581421f};
  model->setOperandValue(param18, param18_init, sizeof(_Float16) * 1);
  static _Float16 param19_init[] = {0.8999999761581421f};
  model->setOperandValue(param19, param19_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in4, param18, param19, layout}, {out4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in4},
    {out4});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_float16_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::INT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 5, 2, 1});
  // Phase 1, operands
  auto in5 = model->addOperand(&type1);
  auto param20 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out5 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param20_init[] = {2};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {5};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in5, param20, param21, layout}, {out5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in5},
    {out5});
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_relaxed_6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::INT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 5, 2, 1});
  // Phase 1, operands
  auto in5 = model->addOperand(&type1);
  auto param20 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out5 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param20_init[] = {2};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {5};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in5, param20, param21, layout}, {out5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in5},
    {out5});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_relaxed_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_quant8_6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type39(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.25f, 100);
  OperandType type53(Type::TENSOR_QUANT8_ASYMM, {1, 5, 2, 1}, 0.25f, 100);
  // Phase 1, operands
  auto in5 = model->addOperand(&type39);
  auto param20 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out5 = model->addOperand(&type53);
  // Phase 2, operations
  static int32_t param20_init[] = {2};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {5};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in5, param20, param21, layout}, {out5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in5},
    {out5});
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_quant8_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_float16_6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type21(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type3(Type::INT32, {});
  OperandType type54(Type::TENSOR_FLOAT16, {1, 5, 2, 1});
  // Phase 1, operands
  auto in5 = model->addOperand(&type21);
  auto param20 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out5 = model->addOperand(&type54);
  // Phase 2, operations
  static int32_t param20_init[] = {2};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {5};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in5, param20, param21, layout}, {out5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in5},
    {out5});
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_float16_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type3(Type::INT32, {});
  OperandType type55(Type::TENSOR_FLOAT32, {1, 1, 5, 2});
  // Phase 1, operands
  auto in5 = model->addOperand(&type23);
  auto param20 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out5 = model->addOperand(&type55);
  // Phase 2, operations
  static int32_t param20_init[] = {2};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {5};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in5, param20, param21, layout}, {out5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in5},
    {out5});
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_relaxed_6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type3(Type::INT32, {});
  OperandType type55(Type::TENSOR_FLOAT32, {1, 1, 5, 2});
  // Phase 1, operands
  auto in5 = model->addOperand(&type23);
  auto param20 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out5 = model->addOperand(&type55);
  // Phase 2, operations
  static int32_t param20_init[] = {2};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {5};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in5, param20, param21, layout}, {out5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in5},
    {out5});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_relaxed_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_quant8_6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type41(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 2}, 0.25f, 100);
  OperandType type56(Type::TENSOR_QUANT8_ASYMM, {1, 1, 5, 2}, 0.25f, 100);
  // Phase 1, operands
  auto in5 = model->addOperand(&type41);
  auto param20 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out5 = model->addOperand(&type56);
  // Phase 2, operations
  static int32_t param20_init[] = {2};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {5};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in5, param20, param21, layout}, {out5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in5},
    {out5});
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_quant8_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_float16_6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type25(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  OperandType type3(Type::INT32, {});
  OperandType type57(Type::TENSOR_FLOAT16, {1, 1, 5, 2});
  // Phase 1, operands
  auto in5 = model->addOperand(&type25);
  auto param20 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out5 = model->addOperand(&type57);
  // Phase 2, operations
  static int32_t param20_init[] = {2};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {5};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in5, param20, param21, layout}, {out5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in5},
    {out5});
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_float16_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in5 = model->addOperand(&type1);
  auto param20 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out5 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param20_init[] = {2};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {5};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in5, param20, param21, layout}, {out5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in5},
    {out5});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_relaxed_6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in5 = model->addOperand(&type1);
  auto param20 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out5 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param20_init[] = {2};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {5};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in5, param20, param21, layout}, {out5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in5},
    {out5});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_relaxed_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_quant8_6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type39(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.25f, 100);
  OperandType type42(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 100);
  // Phase 1, operands
  auto in5 = model->addOperand(&type39);
  auto param20 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out5 = model->addOperand(&type42);
  // Phase 2, operations
  static int32_t param20_init[] = {2};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {5};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in5, param20, param21, layout}, {out5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in5},
    {out5});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_quant8_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_float16_6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type21(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in5 = model->addOperand(&type21);
  auto param20 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out5 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param20_init[] = {2};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {5};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in5, param20, param21, layout}, {out5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in5},
    {out5});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_float16_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in5 = model->addOperand(&type23);
  auto param20 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out5 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param20_init[] = {2};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {5};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in5, param20, param21, layout}, {out5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in5},
    {out5});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_relaxed_6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in5 = model->addOperand(&type23);
  auto param20 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out5 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param20_init[] = {2};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {5};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in5, param20, param21, layout}, {out5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in5},
    {out5});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_relaxed_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_quant8_6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type41(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 2}, 0.25f, 100);
  OperandType type42(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 100);
  // Phase 1, operands
  auto in5 = model->addOperand(&type41);
  auto param20 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out5 = model->addOperand(&type42);
  // Phase 2, operations
  static int32_t param20_init[] = {2};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {5};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in5, param20, param21, layout}, {out5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in5},
    {out5});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_quant8_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_float16_6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type25(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in5 = model->addOperand(&type25);
  auto param20 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out5 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param20_init[] = {2};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {5};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in5, param20, param21, layout}, {out5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in5},
    {out5});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_float16_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type4(Type::FLOAT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 5, 2, 1});
  // Phase 1, operands
  auto in5 = model->addOperand(&type1);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out5 = model->addOperand(&type8);
  // Phase 2, operations
  static float param22_init[] = {1.4f};
  model->setOperandValue(param22, param22_init, sizeof(float) * 1);
  static float param23_init[] = {2.8f};
  model->setOperandValue(param23, param23_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in5, param22, param23, layout}, {out5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in5},
    {out5});
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_relaxed_6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type4(Type::FLOAT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 5, 2, 1});
  // Phase 1, operands
  auto in5 = model->addOperand(&type1);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out5 = model->addOperand(&type8);
  // Phase 2, operations
  static float param22_init[] = {1.4f};
  model->setOperandValue(param22, param22_init, sizeof(float) * 1);
  static float param23_init[] = {2.8f};
  model->setOperandValue(param23, param23_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in5, param22, param23, layout}, {out5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in5},
    {out5});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_relaxed_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_quant8_6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type39(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.25f, 100);
  OperandType type4(Type::FLOAT32, {});
  OperandType type53(Type::TENSOR_QUANT8_ASYMM, {1, 5, 2, 1}, 0.25f, 100);
  // Phase 1, operands
  auto in5 = model->addOperand(&type39);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out5 = model->addOperand(&type53);
  // Phase 2, operations
  static float param22_init[] = {1.4f};
  model->setOperandValue(param22, param22_init, sizeof(float) * 1);
  static float param23_init[] = {2.8f};
  model->setOperandValue(param23, param23_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in5, param22, param23, layout}, {out5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in5},
    {out5});
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_quant8_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_float16_6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type21(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type29(Type::FLOAT16, {});
  OperandType type54(Type::TENSOR_FLOAT16, {1, 5, 2, 1});
  // Phase 1, operands
  auto in5 = model->addOperand(&type21);
  auto param22 = model->addOperand(&type29);
  auto param23 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto out5 = model->addOperand(&type54);
  // Phase 2, operations
  static _Float16 param22_init[] = {1.399999976158142f};
  model->setOperandValue(param22, param22_init, sizeof(_Float16) * 1);
  static _Float16 param23_init[] = {2.799999952316284f};
  model->setOperandValue(param23, param23_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in5, param22, param23, layout}, {out5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in5},
    {out5});
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_float16_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type4(Type::FLOAT32, {});
  OperandType type55(Type::TENSOR_FLOAT32, {1, 1, 5, 2});
  // Phase 1, operands
  auto in5 = model->addOperand(&type23);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out5 = model->addOperand(&type55);
  // Phase 2, operations
  static float param22_init[] = {1.4f};
  model->setOperandValue(param22, param22_init, sizeof(float) * 1);
  static float param23_init[] = {2.8f};
  model->setOperandValue(param23, param23_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in5, param22, param23, layout}, {out5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in5},
    {out5});
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_relaxed_6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type4(Type::FLOAT32, {});
  OperandType type55(Type::TENSOR_FLOAT32, {1, 1, 5, 2});
  // Phase 1, operands
  auto in5 = model->addOperand(&type23);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out5 = model->addOperand(&type55);
  // Phase 2, operations
  static float param22_init[] = {1.4f};
  model->setOperandValue(param22, param22_init, sizeof(float) * 1);
  static float param23_init[] = {2.8f};
  model->setOperandValue(param23, param23_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in5, param22, param23, layout}, {out5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in5},
    {out5});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_relaxed_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_quant8_6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type41(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 2}, 0.25f, 100);
  OperandType type56(Type::TENSOR_QUANT8_ASYMM, {1, 1, 5, 2}, 0.25f, 100);
  // Phase 1, operands
  auto in5 = model->addOperand(&type41);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out5 = model->addOperand(&type56);
  // Phase 2, operations
  static float param22_init[] = {1.4f};
  model->setOperandValue(param22, param22_init, sizeof(float) * 1);
  static float param23_init[] = {2.8f};
  model->setOperandValue(param23, param23_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in5, param22, param23, layout}, {out5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in5},
    {out5});
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_quant8_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_float16_6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type25(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  OperandType type29(Type::FLOAT16, {});
  OperandType type57(Type::TENSOR_FLOAT16, {1, 1, 5, 2});
  // Phase 1, operands
  auto in5 = model->addOperand(&type25);
  auto param22 = model->addOperand(&type29);
  auto param23 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto out5 = model->addOperand(&type57);
  // Phase 2, operations
  static _Float16 param22_init[] = {1.399999976158142f};
  model->setOperandValue(param22, param22_init, sizeof(_Float16) * 1);
  static _Float16 param23_init[] = {2.799999952316284f};
  model->setOperandValue(param23, param23_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in5, param22, param23, layout}, {out5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in5},
    {out5});
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_float16_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in5 = model->addOperand(&type1);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out5 = model->addOperand(&type26);
  // Phase 2, operations
  static float param22_init[] = {1.4f};
  model->setOperandValue(param22, param22_init, sizeof(float) * 1);
  static float param23_init[] = {2.8f};
  model->setOperandValue(param23, param23_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in5, param22, param23, layout}, {out5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in5},
    {out5});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_relaxed_6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in5 = model->addOperand(&type1);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out5 = model->addOperand(&type26);
  // Phase 2, operations
  static float param22_init[] = {1.4f};
  model->setOperandValue(param22, param22_init, sizeof(float) * 1);
  static float param23_init[] = {2.8f};
  model->setOperandValue(param23, param23_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in5, param22, param23, layout}, {out5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in5},
    {out5});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_relaxed_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_quant8_6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type39(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.25f, 100);
  OperandType type4(Type::FLOAT32, {});
  OperandType type42(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 100);
  // Phase 1, operands
  auto in5 = model->addOperand(&type39);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out5 = model->addOperand(&type42);
  // Phase 2, operations
  static float param22_init[] = {1.4f};
  model->setOperandValue(param22, param22_init, sizeof(float) * 1);
  static float param23_init[] = {2.8f};
  model->setOperandValue(param23, param23_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in5, param22, param23, layout}, {out5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in5},
    {out5});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_quant8_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_float16_6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type21(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type29(Type::FLOAT16, {});
  // Phase 1, operands
  auto in5 = model->addOperand(&type21);
  auto param22 = model->addOperand(&type29);
  auto param23 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto out5 = model->addOperand(&type28);
  // Phase 2, operations
  static _Float16 param22_init[] = {1.399999976158142f};
  model->setOperandValue(param22, param22_init, sizeof(_Float16) * 1);
  static _Float16 param23_init[] = {2.799999952316284f};
  model->setOperandValue(param23, param23_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in5, param22, param23, layout}, {out5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in5},
    {out5});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_float16_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in5 = model->addOperand(&type23);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out5 = model->addOperand(&type26);
  // Phase 2, operations
  static float param22_init[] = {1.4f};
  model->setOperandValue(param22, param22_init, sizeof(float) * 1);
  static float param23_init[] = {2.8f};
  model->setOperandValue(param23, param23_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in5, param22, param23, layout}, {out5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in5},
    {out5});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_relaxed_6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in5 = model->addOperand(&type23);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out5 = model->addOperand(&type26);
  // Phase 2, operations
  static float param22_init[] = {1.4f};
  model->setOperandValue(param22, param22_init, sizeof(float) * 1);
  static float param23_init[] = {2.8f};
  model->setOperandValue(param23, param23_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in5, param22, param23, layout}, {out5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in5},
    {out5});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_relaxed_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_quant8_6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type41(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 2}, 0.25f, 100);
  OperandType type42(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 100);
  // Phase 1, operands
  auto in5 = model->addOperand(&type41);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out5 = model->addOperand(&type42);
  // Phase 2, operations
  static float param22_init[] = {1.4f};
  model->setOperandValue(param22, param22_init, sizeof(float) * 1);
  static float param23_init[] = {2.8f};
  model->setOperandValue(param23, param23_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in5, param22, param23, layout}, {out5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in5},
    {out5});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_quant8_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_float16_6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type25(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type29(Type::FLOAT16, {});
  // Phase 1, operands
  auto in5 = model->addOperand(&type25);
  auto param22 = model->addOperand(&type29);
  auto param23 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto out5 = model->addOperand(&type28);
  // Phase 2, operations
  static _Float16 param22_init[] = {1.399999976158142f};
  model->setOperandValue(param22, param22_init, sizeof(_Float16) * 1);
  static _Float16 param23_init[] = {2.799999952316284f};
  model->setOperandValue(param23, param23_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in5, param22, param23, layout}, {out5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in5},
    {out5});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_float16_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_7(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 4, 4, 1});
  // Phase 1, operands
  auto in6 = model->addOperand(&type1);
  auto param24 = model->addOperand(&type3);
  auto param25 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out6 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t param24_init[] = {4};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {4};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in6, param24, param25, layout}, {out6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in6},
    {out6});
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_relaxed_7(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 4, 4, 1});
  // Phase 1, operands
  auto in6 = model->addOperand(&type1);
  auto param24 = model->addOperand(&type3);
  auto param25 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out6 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t param24_init[] = {4};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {4};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in6, param24, param25, layout}, {out6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in6},
    {out6});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_relaxed_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_quant8_7(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type39(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.25f, 100);
  OperandType type48(Type::TENSOR_QUANT8_ASYMM, {1, 4, 4, 1}, 0.25f, 100);
  // Phase 1, operands
  auto in6 = model->addOperand(&type39);
  auto param24 = model->addOperand(&type3);
  auto param25 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out6 = model->addOperand(&type48);
  // Phase 2, operations
  static int32_t param24_init[] = {4};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {4};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in6, param24, param25, layout}, {out6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in6},
    {out6});
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_quant8_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_float16_7(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type21(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type3(Type::INT32, {});
  OperandType type49(Type::TENSOR_FLOAT16, {1, 4, 4, 1});
  // Phase 1, operands
  auto in6 = model->addOperand(&type21);
  auto param24 = model->addOperand(&type3);
  auto param25 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out6 = model->addOperand(&type49);
  // Phase 2, operations
  static int32_t param24_init[] = {4};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {4};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in6, param24, param25, layout}, {out6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in6},
    {out6});
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_float16_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_7(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type3(Type::INT32, {});
  OperandType type50(Type::TENSOR_FLOAT32, {1, 1, 4, 4});
  // Phase 1, operands
  auto in6 = model->addOperand(&type23);
  auto param24 = model->addOperand(&type3);
  auto param25 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out6 = model->addOperand(&type50);
  // Phase 2, operations
  static int32_t param24_init[] = {4};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {4};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in6, param24, param25, layout}, {out6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in6},
    {out6});
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_relaxed_7(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type3(Type::INT32, {});
  OperandType type50(Type::TENSOR_FLOAT32, {1, 1, 4, 4});
  // Phase 1, operands
  auto in6 = model->addOperand(&type23);
  auto param24 = model->addOperand(&type3);
  auto param25 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out6 = model->addOperand(&type50);
  // Phase 2, operations
  static int32_t param24_init[] = {4};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {4};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in6, param24, param25, layout}, {out6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in6},
    {out6});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_relaxed_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_quant8_7(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type41(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 2}, 0.25f, 100);
  OperandType type51(Type::TENSOR_QUANT8_ASYMM, {1, 1, 4, 4}, 0.25f, 100);
  // Phase 1, operands
  auto in6 = model->addOperand(&type41);
  auto param24 = model->addOperand(&type3);
  auto param25 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out6 = model->addOperand(&type51);
  // Phase 2, operations
  static int32_t param24_init[] = {4};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {4};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in6, param24, param25, layout}, {out6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in6},
    {out6});
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_quant8_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_float16_7(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type25(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  OperandType type3(Type::INT32, {});
  OperandType type52(Type::TENSOR_FLOAT16, {1, 1, 4, 4});
  // Phase 1, operands
  auto in6 = model->addOperand(&type25);
  auto param24 = model->addOperand(&type3);
  auto param25 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out6 = model->addOperand(&type52);
  // Phase 2, operations
  static int32_t param24_init[] = {4};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {4};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in6, param24, param25, layout}, {out6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in6},
    {out6});
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_float16_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_7(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in6 = model->addOperand(&type1);
  auto param24 = model->addOperand(&type3);
  auto param25 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out6 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param24_init[] = {4};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {4};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in6, param24, param25, layout}, {out6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in6},
    {out6});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_relaxed_7(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in6 = model->addOperand(&type1);
  auto param24 = model->addOperand(&type3);
  auto param25 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out6 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param24_init[] = {4};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {4};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in6, param24, param25, layout}, {out6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in6},
    {out6});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_relaxed_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_quant8_7(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type39(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.25f, 100);
  OperandType type42(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 100);
  // Phase 1, operands
  auto in6 = model->addOperand(&type39);
  auto param24 = model->addOperand(&type3);
  auto param25 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out6 = model->addOperand(&type42);
  // Phase 2, operations
  static int32_t param24_init[] = {4};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {4};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in6, param24, param25, layout}, {out6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in6},
    {out6});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_quant8_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_float16_7(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type21(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in6 = model->addOperand(&type21);
  auto param24 = model->addOperand(&type3);
  auto param25 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out6 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param24_init[] = {4};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {4};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in6, param24, param25, layout}, {out6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in6},
    {out6});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_float16_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_7(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in6 = model->addOperand(&type23);
  auto param24 = model->addOperand(&type3);
  auto param25 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out6 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param24_init[] = {4};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {4};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in6, param24, param25, layout}, {out6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in6},
    {out6});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_relaxed_7(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in6 = model->addOperand(&type23);
  auto param24 = model->addOperand(&type3);
  auto param25 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out6 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param24_init[] = {4};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {4};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in6, param24, param25, layout}, {out6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in6},
    {out6});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_relaxed_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_quant8_7(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type41(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 2}, 0.25f, 100);
  OperandType type42(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 100);
  // Phase 1, operands
  auto in6 = model->addOperand(&type41);
  auto param24 = model->addOperand(&type3);
  auto param25 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out6 = model->addOperand(&type42);
  // Phase 2, operations
  static int32_t param24_init[] = {4};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {4};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in6, param24, param25, layout}, {out6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in6},
    {out6});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_quant8_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_float16_7(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type25(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto in6 = model->addOperand(&type25);
  auto param24 = model->addOperand(&type3);
  auto param25 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out6 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param24_init[] = {4};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {4};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in6, param24, param25, layout}, {out6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in6},
    {out6});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_float16_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_7(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type4(Type::FLOAT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 4, 4, 1});
  // Phase 1, operands
  auto in6 = model->addOperand(&type1);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out6 = model->addOperand(&type7);
  // Phase 2, operations
  static float param26_init[] = {2.0f};
  model->setOperandValue(param26, param26_init, sizeof(float) * 1);
  static float param27_init[] = {2.0f};
  model->setOperandValue(param27, param27_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in6, param26, param27, layout}, {out6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in6},
    {out6});
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_relaxed_7(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type4(Type::FLOAT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 4, 4, 1});
  // Phase 1, operands
  auto in6 = model->addOperand(&type1);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out6 = model->addOperand(&type7);
  // Phase 2, operations
  static float param26_init[] = {2.0f};
  model->setOperandValue(param26, param26_init, sizeof(float) * 1);
  static float param27_init[] = {2.0f};
  model->setOperandValue(param27, param27_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in6, param26, param27, layout}, {out6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in6},
    {out6});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_relaxed_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_quant8_7(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type39(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.25f, 100);
  OperandType type4(Type::FLOAT32, {});
  OperandType type48(Type::TENSOR_QUANT8_ASYMM, {1, 4, 4, 1}, 0.25f, 100);
  // Phase 1, operands
  auto in6 = model->addOperand(&type39);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out6 = model->addOperand(&type48);
  // Phase 2, operations
  static float param26_init[] = {2.0f};
  model->setOperandValue(param26, param26_init, sizeof(float) * 1);
  static float param27_init[] = {2.0f};
  model->setOperandValue(param27, param27_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in6, param26, param27, layout}, {out6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in6},
    {out6});
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_quant8_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_float16_7(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type21(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type29(Type::FLOAT16, {});
  OperandType type49(Type::TENSOR_FLOAT16, {1, 4, 4, 1});
  // Phase 1, operands
  auto in6 = model->addOperand(&type21);
  auto param26 = model->addOperand(&type29);
  auto param27 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto out6 = model->addOperand(&type49);
  // Phase 2, operations
  static _Float16 param26_init[] = {2.0f};
  model->setOperandValue(param26, param26_init, sizeof(_Float16) * 1);
  static _Float16 param27_init[] = {2.0f};
  model->setOperandValue(param27, param27_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in6, param26, param27, layout}, {out6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in6},
    {out6});
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_float16_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_7(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type4(Type::FLOAT32, {});
  OperandType type50(Type::TENSOR_FLOAT32, {1, 1, 4, 4});
  // Phase 1, operands
  auto in6 = model->addOperand(&type23);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out6 = model->addOperand(&type50);
  // Phase 2, operations
  static float param26_init[] = {2.0f};
  model->setOperandValue(param26, param26_init, sizeof(float) * 1);
  static float param27_init[] = {2.0f};
  model->setOperandValue(param27, param27_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in6, param26, param27, layout}, {out6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in6},
    {out6});
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_relaxed_7(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type4(Type::FLOAT32, {});
  OperandType type50(Type::TENSOR_FLOAT32, {1, 1, 4, 4});
  // Phase 1, operands
  auto in6 = model->addOperand(&type23);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out6 = model->addOperand(&type50);
  // Phase 2, operations
  static float param26_init[] = {2.0f};
  model->setOperandValue(param26, param26_init, sizeof(float) * 1);
  static float param27_init[] = {2.0f};
  model->setOperandValue(param27, param27_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in6, param26, param27, layout}, {out6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in6},
    {out6});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_relaxed_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_quant8_7(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type41(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 2}, 0.25f, 100);
  OperandType type51(Type::TENSOR_QUANT8_ASYMM, {1, 1, 4, 4}, 0.25f, 100);
  // Phase 1, operands
  auto in6 = model->addOperand(&type41);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out6 = model->addOperand(&type51);
  // Phase 2, operations
  static float param26_init[] = {2.0f};
  model->setOperandValue(param26, param26_init, sizeof(float) * 1);
  static float param27_init[] = {2.0f};
  model->setOperandValue(param27, param27_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in6, param26, param27, layout}, {out6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in6},
    {out6});
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_quant8_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_float16_7(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type25(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  OperandType type29(Type::FLOAT16, {});
  OperandType type52(Type::TENSOR_FLOAT16, {1, 1, 4, 4});
  // Phase 1, operands
  auto in6 = model->addOperand(&type25);
  auto param26 = model->addOperand(&type29);
  auto param27 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto out6 = model->addOperand(&type52);
  // Phase 2, operations
  static _Float16 param26_init[] = {2.0f};
  model->setOperandValue(param26, param26_init, sizeof(_Float16) * 1);
  static _Float16 param27_init[] = {2.0f};
  model->setOperandValue(param27, param27_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in6, param26, param27, layout}, {out6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in6},
    {out6});
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_float16_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_7(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in6 = model->addOperand(&type1);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out6 = model->addOperand(&type26);
  // Phase 2, operations
  static float param26_init[] = {2.0f};
  model->setOperandValue(param26, param26_init, sizeof(float) * 1);
  static float param27_init[] = {2.0f};
  model->setOperandValue(param27, param27_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in6, param26, param27, layout}, {out6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in6},
    {out6});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_relaxed_7(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in6 = model->addOperand(&type1);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out6 = model->addOperand(&type26);
  // Phase 2, operations
  static float param26_init[] = {2.0f};
  model->setOperandValue(param26, param26_init, sizeof(float) * 1);
  static float param27_init[] = {2.0f};
  model->setOperandValue(param27, param27_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in6, param26, param27, layout}, {out6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in6},
    {out6});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_relaxed_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_quant8_7(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type39(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.25f, 100);
  OperandType type4(Type::FLOAT32, {});
  OperandType type42(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 100);
  // Phase 1, operands
  auto in6 = model->addOperand(&type39);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out6 = model->addOperand(&type42);
  // Phase 2, operations
  static float param26_init[] = {2.0f};
  model->setOperandValue(param26, param26_init, sizeof(float) * 1);
  static float param27_init[] = {2.0f};
  model->setOperandValue(param27, param27_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in6, param26, param27, layout}, {out6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in6},
    {out6});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_quant8_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_float16_7(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type21(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type29(Type::FLOAT16, {});
  // Phase 1, operands
  auto in6 = model->addOperand(&type21);
  auto param26 = model->addOperand(&type29);
  auto param27 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto out6 = model->addOperand(&type28);
  // Phase 2, operations
  static _Float16 param26_init[] = {2.0f};
  model->setOperandValue(param26, param26_init, sizeof(_Float16) * 1);
  static _Float16 param27_init[] = {2.0f};
  model->setOperandValue(param27, param27_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in6, param26, param27, layout}, {out6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in6},
    {out6});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_float16_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_7(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in6 = model->addOperand(&type23);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out6 = model->addOperand(&type26);
  // Phase 2, operations
  static float param26_init[] = {2.0f};
  model->setOperandValue(param26, param26_init, sizeof(float) * 1);
  static float param27_init[] = {2.0f};
  model->setOperandValue(param27, param27_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in6, param26, param27, layout}, {out6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in6},
    {out6});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_relaxed_7(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto in6 = model->addOperand(&type23);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out6 = model->addOperand(&type26);
  // Phase 2, operations
  static float param26_init[] = {2.0f};
  model->setOperandValue(param26, param26_init, sizeof(float) * 1);
  static float param27_init[] = {2.0f};
  model->setOperandValue(param27, param27_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in6, param26, param27, layout}, {out6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in6},
    {out6});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_relaxed_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_quant8_7(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type41(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 2}, 0.25f, 100);
  OperandType type42(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 100);
  // Phase 1, operands
  auto in6 = model->addOperand(&type41);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out6 = model->addOperand(&type42);
  // Phase 2, operations
  static float param26_init[] = {2.0f};
  model->setOperandValue(param26, param26_init, sizeof(float) * 1);
  static float param27_init[] = {2.0f};
  model->setOperandValue(param27, param27_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in6, param26, param27, layout}, {out6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in6},
    {out6});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_quant8_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_float16_7(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type25(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type29(Type::FLOAT16, {});
  // Phase 1, operands
  auto in6 = model->addOperand(&type25);
  auto param26 = model->addOperand(&type29);
  auto param27 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto out6 = model->addOperand(&type28);
  // Phase 2, operations
  static _Float16 param26_init[] = {2.0f};
  model->setOperandValue(param26, param26_init, sizeof(_Float16) * 1);
  static _Float16 param27_init[] = {2.0f};
  model->setOperandValue(param27, param27_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in6, param26, param27, layout}, {out6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in6},
    {out6});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_float16_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {2, 3, 3, 2});
  OperandType type3(Type::INT32, {});
  OperandType type9(Type::TENSOR_FLOAT32, {2, 2, 2, 2});
  // Phase 1, operands
  auto in7 = model->addOperand(&type9);
  auto param28 = model->addOperand(&type3);
  auto param29 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out7 = model->addOperand(&type10);
  // Phase 2, operations
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in7, param28, param29, layout}, {out7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in7},
    {out7});
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_relaxed_8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {2, 3, 3, 2});
  OperandType type3(Type::INT32, {});
  OperandType type9(Type::TENSOR_FLOAT32, {2, 2, 2, 2});
  // Phase 1, operands
  auto in7 = model->addOperand(&type9);
  auto param28 = model->addOperand(&type3);
  auto param29 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out7 = model->addOperand(&type10);
  // Phase 2, operations
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in7, param28, param29, layout}, {out7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in7},
    {out7});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_relaxed_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_quant8_8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type58(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 2}, 0.25f, 100);
  OperandType type59(Type::TENSOR_QUANT8_ASYMM, {2, 3, 3, 2}, 0.25f, 100);
  // Phase 1, operands
  auto in7 = model->addOperand(&type58);
  auto param28 = model->addOperand(&type3);
  auto param29 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out7 = model->addOperand(&type59);
  // Phase 2, operations
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in7, param28, param29, layout}, {out7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in7},
    {out7});
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_quant8_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_float16_8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type60(Type::TENSOR_FLOAT16, {2, 2, 2, 2});
  OperandType type61(Type::TENSOR_FLOAT16, {2, 3, 3, 2});
  // Phase 1, operands
  auto in7 = model->addOperand(&type60);
  auto param28 = model->addOperand(&type3);
  auto param29 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out7 = model->addOperand(&type61);
  // Phase 2, operations
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in7, param28, param29, layout}, {out7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in7},
    {out7});
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_float16_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type62(Type::TENSOR_FLOAT32, {2, 2, 3, 3});
  OperandType type9(Type::TENSOR_FLOAT32, {2, 2, 2, 2});
  // Phase 1, operands
  auto in7 = model->addOperand(&type9);
  auto param28 = model->addOperand(&type3);
  auto param29 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out7 = model->addOperand(&type62);
  // Phase 2, operations
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in7, param28, param29, layout}, {out7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in7},
    {out7});
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_relaxed_8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type62(Type::TENSOR_FLOAT32, {2, 2, 3, 3});
  OperandType type9(Type::TENSOR_FLOAT32, {2, 2, 2, 2});
  // Phase 1, operands
  auto in7 = model->addOperand(&type9);
  auto param28 = model->addOperand(&type3);
  auto param29 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out7 = model->addOperand(&type62);
  // Phase 2, operations
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in7, param28, param29, layout}, {out7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in7},
    {out7});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_relaxed_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_quant8_8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type58(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 2}, 0.25f, 100);
  OperandType type63(Type::TENSOR_QUANT8_ASYMM, {2, 2, 3, 3}, 0.25f, 100);
  // Phase 1, operands
  auto in7 = model->addOperand(&type58);
  auto param28 = model->addOperand(&type3);
  auto param29 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out7 = model->addOperand(&type63);
  // Phase 2, operations
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in7, param28, param29, layout}, {out7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in7},
    {out7});
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_quant8_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_float16_8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type60(Type::TENSOR_FLOAT16, {2, 2, 2, 2});
  OperandType type64(Type::TENSOR_FLOAT16, {2, 2, 3, 3});
  // Phase 1, operands
  auto in7 = model->addOperand(&type60);
  auto param28 = model->addOperand(&type3);
  auto param29 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out7 = model->addOperand(&type64);
  // Phase 2, operations
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in7, param28, param29, layout}, {out7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in7},
    {out7});
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_float16_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type9(Type::TENSOR_FLOAT32, {2, 2, 2, 2});
  // Phase 1, operands
  auto in7 = model->addOperand(&type9);
  auto param28 = model->addOperand(&type3);
  auto param29 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out7 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in7, param28, param29, layout}, {out7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in7},
    {out7});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_relaxed_8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type9(Type::TENSOR_FLOAT32, {2, 2, 2, 2});
  // Phase 1, operands
  auto in7 = model->addOperand(&type9);
  auto param28 = model->addOperand(&type3);
  auto param29 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out7 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in7, param28, param29, layout}, {out7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in7},
    {out7});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_relaxed_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_quant8_8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type42(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 100);
  OperandType type58(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 2}, 0.25f, 100);
  // Phase 1, operands
  auto in7 = model->addOperand(&type58);
  auto param28 = model->addOperand(&type3);
  auto param29 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out7 = model->addOperand(&type42);
  // Phase 2, operations
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in7, param28, param29, layout}, {out7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in7},
    {out7});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_quant8_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_float16_8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type60(Type::TENSOR_FLOAT16, {2, 2, 2, 2});
  // Phase 1, operands
  auto in7 = model->addOperand(&type60);
  auto param28 = model->addOperand(&type3);
  auto param29 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out7 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in7, param28, param29, layout}, {out7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in7},
    {out7});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_float16_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type9(Type::TENSOR_FLOAT32, {2, 2, 2, 2});
  // Phase 1, operands
  auto in7 = model->addOperand(&type9);
  auto param28 = model->addOperand(&type3);
  auto param29 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out7 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in7, param28, param29, layout}, {out7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in7},
    {out7});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_relaxed_8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type9(Type::TENSOR_FLOAT32, {2, 2, 2, 2});
  // Phase 1, operands
  auto in7 = model->addOperand(&type9);
  auto param28 = model->addOperand(&type3);
  auto param29 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out7 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in7, param28, param29, layout}, {out7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in7},
    {out7});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_relaxed_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_quant8_8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type42(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 100);
  OperandType type58(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 2}, 0.25f, 100);
  // Phase 1, operands
  auto in7 = model->addOperand(&type58);
  auto param28 = model->addOperand(&type3);
  auto param29 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out7 = model->addOperand(&type42);
  // Phase 2, operations
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in7, param28, param29, layout}, {out7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in7},
    {out7});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_quant8_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_float16_8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type60(Type::TENSOR_FLOAT16, {2, 2, 2, 2});
  // Phase 1, operands
  auto in7 = model->addOperand(&type60);
  auto param28 = model->addOperand(&type3);
  auto param29 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto out7 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in7, param28, param29, layout}, {out7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in7},
    {out7});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_float16_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {2, 3, 3, 2});
  OperandType type4(Type::FLOAT32, {});
  OperandType type9(Type::TENSOR_FLOAT32, {2, 2, 2, 2});
  // Phase 1, operands
  auto in7 = model->addOperand(&type9);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out7 = model->addOperand(&type10);
  // Phase 2, operations
  static float param30_init[] = {1.6f};
  model->setOperandValue(param30, param30_init, sizeof(float) * 1);
  static float param31_init[] = {1.8f};
  model->setOperandValue(param31, param31_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in7, param30, param31, layout}, {out7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in7},
    {out7});
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_relaxed_8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {2, 3, 3, 2});
  OperandType type4(Type::FLOAT32, {});
  OperandType type9(Type::TENSOR_FLOAT32, {2, 2, 2, 2});
  // Phase 1, operands
  auto in7 = model->addOperand(&type9);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out7 = model->addOperand(&type10);
  // Phase 2, operations
  static float param30_init[] = {1.6f};
  model->setOperandValue(param30, param30_init, sizeof(float) * 1);
  static float param31_init[] = {1.8f};
  model->setOperandValue(param31, param31_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in7, param30, param31, layout}, {out7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in7},
    {out7});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_relaxed_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_quant8_8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type58(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 2}, 0.25f, 100);
  OperandType type59(Type::TENSOR_QUANT8_ASYMM, {2, 3, 3, 2}, 0.25f, 100);
  // Phase 1, operands
  auto in7 = model->addOperand(&type58);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out7 = model->addOperand(&type59);
  // Phase 2, operations
  static float param30_init[] = {1.6f};
  model->setOperandValue(param30, param30_init, sizeof(float) * 1);
  static float param31_init[] = {1.8f};
  model->setOperandValue(param31, param31_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in7, param30, param31, layout}, {out7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in7},
    {out7});
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_quant8_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_float16_8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type29(Type::FLOAT16, {});
  OperandType type60(Type::TENSOR_FLOAT16, {2, 2, 2, 2});
  OperandType type61(Type::TENSOR_FLOAT16, {2, 3, 3, 2});
  // Phase 1, operands
  auto in7 = model->addOperand(&type60);
  auto param30 = model->addOperand(&type29);
  auto param31 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto out7 = model->addOperand(&type61);
  // Phase 2, operations
  static _Float16 param30_init[] = {1.600000023841858f};
  model->setOperandValue(param30, param30_init, sizeof(_Float16) * 1);
  static _Float16 param31_init[] = {1.7999999523162842f};
  model->setOperandValue(param31, param31_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in7, param30, param31, layout}, {out7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in7},
    {out7});
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_float16_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type62(Type::TENSOR_FLOAT32, {2, 2, 3, 3});
  OperandType type9(Type::TENSOR_FLOAT32, {2, 2, 2, 2});
  // Phase 1, operands
  auto in7 = model->addOperand(&type9);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out7 = model->addOperand(&type62);
  // Phase 2, operations
  static float param30_init[] = {1.6f};
  model->setOperandValue(param30, param30_init, sizeof(float) * 1);
  static float param31_init[] = {1.8f};
  model->setOperandValue(param31, param31_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in7, param30, param31, layout}, {out7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in7},
    {out7});
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_relaxed_8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type62(Type::TENSOR_FLOAT32, {2, 2, 3, 3});
  OperandType type9(Type::TENSOR_FLOAT32, {2, 2, 2, 2});
  // Phase 1, operands
  auto in7 = model->addOperand(&type9);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out7 = model->addOperand(&type62);
  // Phase 2, operations
  static float param30_init[] = {1.6f};
  model->setOperandValue(param30, param30_init, sizeof(float) * 1);
  static float param31_init[] = {1.8f};
  model->setOperandValue(param31, param31_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in7, param30, param31, layout}, {out7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in7},
    {out7});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_relaxed_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_quant8_8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type58(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 2}, 0.25f, 100);
  OperandType type63(Type::TENSOR_QUANT8_ASYMM, {2, 2, 3, 3}, 0.25f, 100);
  // Phase 1, operands
  auto in7 = model->addOperand(&type58);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out7 = model->addOperand(&type63);
  // Phase 2, operations
  static float param30_init[] = {1.6f};
  model->setOperandValue(param30, param30_init, sizeof(float) * 1);
  static float param31_init[] = {1.8f};
  model->setOperandValue(param31, param31_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in7, param30, param31, layout}, {out7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in7},
    {out7});
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_quant8_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_float16_8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type29(Type::FLOAT16, {});
  OperandType type60(Type::TENSOR_FLOAT16, {2, 2, 2, 2});
  OperandType type64(Type::TENSOR_FLOAT16, {2, 2, 3, 3});
  // Phase 1, operands
  auto in7 = model->addOperand(&type60);
  auto param30 = model->addOperand(&type29);
  auto param31 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto out7 = model->addOperand(&type64);
  // Phase 2, operations
  static _Float16 param30_init[] = {1.600000023841858f};
  model->setOperandValue(param30, param30_init, sizeof(_Float16) * 1);
  static _Float16 param31_init[] = {1.7999999523162842f};
  model->setOperandValue(param31, param31_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in7, param30, param31, layout}, {out7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in7},
    {out7});
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_float16_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  OperandType type9(Type::TENSOR_FLOAT32, {2, 2, 2, 2});
  // Phase 1, operands
  auto in7 = model->addOperand(&type9);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out7 = model->addOperand(&type26);
  // Phase 2, operations
  static float param30_init[] = {1.6f};
  model->setOperandValue(param30, param30_init, sizeof(float) * 1);
  static float param31_init[] = {1.8f};
  model->setOperandValue(param31, param31_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in7, param30, param31, layout}, {out7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in7},
    {out7});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_relaxed_8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  OperandType type9(Type::TENSOR_FLOAT32, {2, 2, 2, 2});
  // Phase 1, operands
  auto in7 = model->addOperand(&type9);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out7 = model->addOperand(&type26);
  // Phase 2, operations
  static float param30_init[] = {1.6f};
  model->setOperandValue(param30, param30_init, sizeof(float) * 1);
  static float param31_init[] = {1.8f};
  model->setOperandValue(param31, param31_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in7, param30, param31, layout}, {out7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in7},
    {out7});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_relaxed_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_quant8_8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type42(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 100);
  OperandType type58(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 2}, 0.25f, 100);
  // Phase 1, operands
  auto in7 = model->addOperand(&type58);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out7 = model->addOperand(&type42);
  // Phase 2, operations
  static float param30_init[] = {1.6f};
  model->setOperandValue(param30, param30_init, sizeof(float) * 1);
  static float param31_init[] = {1.8f};
  model->setOperandValue(param31, param31_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in7, param30, param31, layout}, {out7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in7},
    {out7});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_quant8_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_float16_8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type29(Type::FLOAT16, {});
  OperandType type60(Type::TENSOR_FLOAT16, {2, 2, 2, 2});
  // Phase 1, operands
  auto in7 = model->addOperand(&type60);
  auto param30 = model->addOperand(&type29);
  auto param31 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto out7 = model->addOperand(&type28);
  // Phase 2, operations
  static _Float16 param30_init[] = {1.600000023841858f};
  model->setOperandValue(param30, param30_init, sizeof(_Float16) * 1);
  static _Float16 param31_init[] = {1.7999999523162842f};
  model->setOperandValue(param31, param31_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in7, param30, param31, layout}, {out7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in7},
    {out7});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_float16_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  OperandType type9(Type::TENSOR_FLOAT32, {2, 2, 2, 2});
  // Phase 1, operands
  auto in7 = model->addOperand(&type9);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out7 = model->addOperand(&type26);
  // Phase 2, operations
  static float param30_init[] = {1.6f};
  model->setOperandValue(param30, param30_init, sizeof(float) * 1);
  static float param31_init[] = {1.8f};
  model->setOperandValue(param31, param31_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in7, param30, param31, layout}, {out7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in7},
    {out7});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_relaxed_8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  OperandType type9(Type::TENSOR_FLOAT32, {2, 2, 2, 2});
  // Phase 1, operands
  auto in7 = model->addOperand(&type9);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out7 = model->addOperand(&type26);
  // Phase 2, operations
  static float param30_init[] = {1.6f};
  model->setOperandValue(param30, param30_init, sizeof(float) * 1);
  static float param31_init[] = {1.8f};
  model->setOperandValue(param31, param31_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in7, param30, param31, layout}, {out7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in7},
    {out7});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_relaxed_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_quant8_8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type42(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 100);
  OperandType type58(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 2}, 0.25f, 100);
  // Phase 1, operands
  auto in7 = model->addOperand(&type58);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out7 = model->addOperand(&type42);
  // Phase 2, operations
  static float param30_init[] = {1.6f};
  model->setOperandValue(param30, param30_init, sizeof(float) * 1);
  static float param31_init[] = {1.8f};
  model->setOperandValue(param31, param31_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in7, param30, param31, layout}, {out7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in7},
    {out7});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_quant8_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_float16_8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type29(Type::FLOAT16, {});
  OperandType type60(Type::TENSOR_FLOAT16, {2, 2, 2, 2});
  // Phase 1, operands
  auto in7 = model->addOperand(&type60);
  auto param30 = model->addOperand(&type29);
  auto param31 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto out7 = model->addOperand(&type28);
  // Phase 2, operations
  static _Float16 param30_init[] = {1.600000023841858f};
  model->setOperandValue(param30, param30_init, sizeof(_Float16) * 1);
  static _Float16 param31_init[] = {1.7999999523162842f};
  model->setOperandValue(param31, param31_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {in7, param30, param31, layout}, {out7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in7},
    {out7});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_float16_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type12(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type13(Type::TENSOR_FLOAT32, {0});
  OperandType type14(Type::TENSOR_INT32, {0});
  OperandType type15(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type16(Type::TENSOR_INT32, {1});
  OperandType type17(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type18(Type::TENSOR_FLOAT32, {0, 3, 3, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type11);
  auto roi = model->addOperand(&type12);
  auto param32 = model->addOperand(&type16);
  auto param33 = model->addOperand(&type4);
  auto param34 = model->addOperand(&type3);
  auto param35 = model->addOperand(&type3);
  auto param36 = model->addOperand(&type4);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type4);
  auto scoresOut = model->addOperand(&type13);
  auto roiOut = model->addOperand(&type15);
  auto classesOut = model->addOperand(&type14);
  auto batchSplitOut = model->addOperand(&type14);
  auto in8 = model->addOperand(&type2);
  auto param39 = model->addOperand(&type3);
  auto param40 = model->addOperand(&type3);
  auto param41 = model->addOperand(&type4);
  auto param42 = model->addOperand(&type4);
  auto param43 = model->addOperand(&type3);
  auto param44 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type17);
  auto param45 = model->addOperand(&type3);
  auto param46 = model->addOperand(&type3);
  auto out8 = model->addOperand(&type18);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param32_init[] = {0};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static float param33_init[] = {0.3f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static int32_t param34_init[] = {-1};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {0};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static float param36_init[] = {0.4f};
  model->setOperandValue(param36, param36_init, sizeof(float) * 1);
  static float param37_init[] = {1.0f};
  model->setOperandValue(param37, param37_init, sizeof(float) * 1);
  static float param38_init[] = {0.3f};
  model->setOperandValue(param38, param38_init, sizeof(float) * 1);
  static int32_t param39_init[] = {2};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {2};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static float param41_init[] = {2.0f};
  model->setOperandValue(param41, param41_init, sizeof(float) * 1);
  static float param42_init[] = {2.0f};
  model->setOperandValue(param42, param42_init, sizeof(float) * 1);
  static int32_t param43_init[] = {4};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {4};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param45_init[] = {3};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  static int32_t param46_init[] = {3};
  model->setOperandValue(param46, param46_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param32, param33, param34, param35, param36, param37, param38}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in8, roiOut, batchSplitOut, param39, param40, param41, param42, param43, param44, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {featureMap, param45, param46, layout}, {out8});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in8},
    {scoresOut, classesOut, out8});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type12(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type13(Type::TENSOR_FLOAT32, {0});
  OperandType type14(Type::TENSOR_INT32, {0});
  OperandType type15(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type16(Type::TENSOR_INT32, {1});
  OperandType type17(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type18(Type::TENSOR_FLOAT32, {0, 3, 3, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type11);
  auto roi = model->addOperand(&type12);
  auto param32 = model->addOperand(&type16);
  auto param33 = model->addOperand(&type4);
  auto param34 = model->addOperand(&type3);
  auto param35 = model->addOperand(&type3);
  auto param36 = model->addOperand(&type4);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type4);
  auto scoresOut = model->addOperand(&type13);
  auto roiOut = model->addOperand(&type15);
  auto classesOut = model->addOperand(&type14);
  auto batchSplitOut = model->addOperand(&type14);
  auto in8 = model->addOperand(&type2);
  auto param39 = model->addOperand(&type3);
  auto param40 = model->addOperand(&type3);
  auto param41 = model->addOperand(&type4);
  auto param42 = model->addOperand(&type4);
  auto param43 = model->addOperand(&type3);
  auto param44 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type17);
  auto param45 = model->addOperand(&type3);
  auto param46 = model->addOperand(&type3);
  auto out8 = model->addOperand(&type18);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param32_init[] = {0};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static float param33_init[] = {0.3f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static int32_t param34_init[] = {-1};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {0};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static float param36_init[] = {0.4f};
  model->setOperandValue(param36, param36_init, sizeof(float) * 1);
  static float param37_init[] = {1.0f};
  model->setOperandValue(param37, param37_init, sizeof(float) * 1);
  static float param38_init[] = {0.3f};
  model->setOperandValue(param38, param38_init, sizeof(float) * 1);
  static int32_t param39_init[] = {2};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {2};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static float param41_init[] = {2.0f};
  model->setOperandValue(param41, param41_init, sizeof(float) * 1);
  static float param42_init[] = {2.0f};
  model->setOperandValue(param42, param42_init, sizeof(float) * 1);
  static int32_t param43_init[] = {4};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {4};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param45_init[] = {3};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  static int32_t param46_init[] = {3};
  model->setOperandValue(param46, param46_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param32, param33, param34, param35, param36, param37, param38}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in8, roiOut, batchSplitOut, param39, param40, param41, param42, param43, param44, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {featureMap, param45, param46, layout}, {out8});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in8},
    {scoresOut, classesOut, out8});
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
  OperandType type14(Type::TENSOR_INT32, {0});
  OperandType type16(Type::TENSOR_INT32, {1});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type65(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 1}, 0.1f, 128);
  OperandType type66(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.1f, 128);
  OperandType type67(Type::TENSOR_QUANT8_ASYMM, {0, 3, 3, 1}, 0.1f, 128);
  OperandType type68(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type69(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type70(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type71(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  // Phase 1, operands
  auto scores = model->addOperand(&type70);
  auto roi = model->addOperand(&type68);
  auto param32 = model->addOperand(&type16);
  auto param33 = model->addOperand(&type4);
  auto param34 = model->addOperand(&type3);
  auto param35 = model->addOperand(&type3);
  auto param36 = model->addOperand(&type4);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type4);
  auto scoresOut = model->addOperand(&type71);
  auto roiOut = model->addOperand(&type69);
  auto classesOut = model->addOperand(&type14);
  auto batchSplitOut = model->addOperand(&type14);
  auto in8 = model->addOperand(&type66);
  auto param39 = model->addOperand(&type3);
  auto param40 = model->addOperand(&type3);
  auto param41 = model->addOperand(&type4);
  auto param42 = model->addOperand(&type4);
  auto param43 = model->addOperand(&type3);
  auto param44 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type65);
  auto param45 = model->addOperand(&type3);
  auto param46 = model->addOperand(&type3);
  auto out8 = model->addOperand(&type67);
  // Phase 2, operations
  static uint8_t scores_init[] = {137, 129};
  model->setOperandValue(scores, scores_init, sizeof(uint8_t) * 2);
  static uint16_t roi_init[] = {8, 8, 80, 80, 0, 0, 80, 80};
  model->setOperandValue(roi, roi_init, sizeof(uint16_t) * 8);
  static int32_t param32_init[] = {0};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static float param33_init[] = {0.3f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static int32_t param34_init[] = {-1};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {0};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static float param36_init[] = {0.4f};
  model->setOperandValue(param36, param36_init, sizeof(float) * 1);
  static float param37_init[] = {1.0f};
  model->setOperandValue(param37, param37_init, sizeof(float) * 1);
  static float param38_init[] = {0.3f};
  model->setOperandValue(param38, param38_init, sizeof(float) * 1);
  static int32_t param39_init[] = {2};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {2};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static float param41_init[] = {2.0f};
  model->setOperandValue(param41, param41_init, sizeof(float) * 1);
  static float param42_init[] = {2.0f};
  model->setOperandValue(param42, param42_init, sizeof(float) * 1);
  static int32_t param43_init[] = {4};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {4};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param45_init[] = {3};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  static int32_t param46_init[] = {3};
  model->setOperandValue(param46, param46_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param32, param33, param34, param35, param36, param37, param38}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in8, roiOut, batchSplitOut, param39, param40, param41, param42, param43, param44, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {featureMap, param45, param46, layout}, {out8});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in8},
    {scoresOut, classesOut, out8});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nhwc_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type14(Type::TENSOR_INT32, {0});
  OperandType type16(Type::TENSOR_INT32, {1});
  OperandType type22(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type29(Type::FLOAT16, {});
  OperandType type3(Type::INT32, {});
  OperandType type72(Type::TENSOR_FLOAT16, {0, 2, 2, 1});
  OperandType type73(Type::TENSOR_FLOAT16, {0, 3, 3, 1});
  OperandType type74(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type75(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type76(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type77(Type::TENSOR_FLOAT16, {0});
  // Phase 1, operands
  auto scores = model->addOperand(&type76);
  auto roi = model->addOperand(&type74);
  auto param32 = model->addOperand(&type16);
  auto param33 = model->addOperand(&type29);
  auto param34 = model->addOperand(&type3);
  auto param35 = model->addOperand(&type3);
  auto param36 = model->addOperand(&type29);
  auto param37 = model->addOperand(&type29);
  auto param38 = model->addOperand(&type29);
  auto scoresOut = model->addOperand(&type77);
  auto roiOut = model->addOperand(&type75);
  auto classesOut = model->addOperand(&type14);
  auto batchSplitOut = model->addOperand(&type14);
  auto in8 = model->addOperand(&type22);
  auto param39 = model->addOperand(&type3);
  auto param40 = model->addOperand(&type3);
  auto param41 = model->addOperand(&type29);
  auto param42 = model->addOperand(&type29);
  auto param43 = model->addOperand(&type3);
  auto param44 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type72);
  auto param45 = model->addOperand(&type3);
  auto param46 = model->addOperand(&type3);
  auto out8 = model->addOperand(&type73);
  // Phase 2, operations
  static _Float16 scores_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores, scores_init, sizeof(_Float16) * 2);
  static _Float16 roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(_Float16) * 8);
  static int32_t param32_init[] = {0};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static _Float16 param33_init[] = {0.30000001192092896f};
  model->setOperandValue(param33, param33_init, sizeof(_Float16) * 1);
  static int32_t param34_init[] = {-1};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {0};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static _Float16 param36_init[] = {0.4000000059604645f};
  model->setOperandValue(param36, param36_init, sizeof(_Float16) * 1);
  static _Float16 param37_init[] = {1.0f};
  model->setOperandValue(param37, param37_init, sizeof(_Float16) * 1);
  static _Float16 param38_init[] = {0.30000001192092896f};
  model->setOperandValue(param38, param38_init, sizeof(_Float16) * 1);
  static int32_t param39_init[] = {2};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {2};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static _Float16 param41_init[] = {2.0f};
  model->setOperandValue(param41, param41_init, sizeof(_Float16) * 1);
  static _Float16 param42_init[] = {2.0f};
  model->setOperandValue(param42, param42_init, sizeof(_Float16) * 1);
  static int32_t param43_init[] = {4};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {4};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param45_init[] = {3};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  static int32_t param46_init[] = {3};
  model->setOperandValue(param46, param46_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param32, param33, param34, param35, param36, param37, param38}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in8, roiOut, batchSplitOut, param39, param40, param41, param42, param43, param44, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {featureMap, param45, param46, layout}, {out8});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in8},
    {scoresOut, classesOut, out8});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type12(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type13(Type::TENSOR_FLOAT32, {0});
  OperandType type14(Type::TENSOR_INT32, {0});
  OperandType type15(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type16(Type::TENSOR_INT32, {1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type78(Type::TENSOR_FLOAT32, {0, 1, 2, 2});
  OperandType type79(Type::TENSOR_FLOAT32, {0, 1, 3, 3});
  // Phase 1, operands
  auto scores = model->addOperand(&type11);
  auto roi = model->addOperand(&type12);
  auto param32 = model->addOperand(&type16);
  auto param33 = model->addOperand(&type4);
  auto param34 = model->addOperand(&type3);
  auto param35 = model->addOperand(&type3);
  auto param36 = model->addOperand(&type4);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type4);
  auto scoresOut = model->addOperand(&type13);
  auto roiOut = model->addOperand(&type15);
  auto classesOut = model->addOperand(&type14);
  auto batchSplitOut = model->addOperand(&type14);
  auto in8 = model->addOperand(&type2);
  auto param39 = model->addOperand(&type3);
  auto param40 = model->addOperand(&type3);
  auto param41 = model->addOperand(&type4);
  auto param42 = model->addOperand(&type4);
  auto param43 = model->addOperand(&type3);
  auto param44 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type78);
  auto param45 = model->addOperand(&type3);
  auto param46 = model->addOperand(&type3);
  auto out8 = model->addOperand(&type79);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param32_init[] = {0};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static float param33_init[] = {0.3f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static int32_t param34_init[] = {-1};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {0};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static float param36_init[] = {0.4f};
  model->setOperandValue(param36, param36_init, sizeof(float) * 1);
  static float param37_init[] = {1.0f};
  model->setOperandValue(param37, param37_init, sizeof(float) * 1);
  static float param38_init[] = {0.3f};
  model->setOperandValue(param38, param38_init, sizeof(float) * 1);
  static int32_t param39_init[] = {2};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {2};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static float param41_init[] = {2.0f};
  model->setOperandValue(param41, param41_init, sizeof(float) * 1);
  static float param42_init[] = {2.0f};
  model->setOperandValue(param42, param42_init, sizeof(float) * 1);
  static int32_t param43_init[] = {4};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {4};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param45_init[] = {3};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  static int32_t param46_init[] = {3};
  model->setOperandValue(param46, param46_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param32, param33, param34, param35, param36, param37, param38}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in8, roiOut, batchSplitOut, param39, param40, param41, param42, param43, param44, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {featureMap, param45, param46, layout}, {out8});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in8},
    {scoresOut, classesOut, out8});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type12(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type13(Type::TENSOR_FLOAT32, {0});
  OperandType type14(Type::TENSOR_INT32, {0});
  OperandType type15(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type16(Type::TENSOR_INT32, {1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type78(Type::TENSOR_FLOAT32, {0, 1, 2, 2});
  OperandType type79(Type::TENSOR_FLOAT32, {0, 1, 3, 3});
  // Phase 1, operands
  auto scores = model->addOperand(&type11);
  auto roi = model->addOperand(&type12);
  auto param32 = model->addOperand(&type16);
  auto param33 = model->addOperand(&type4);
  auto param34 = model->addOperand(&type3);
  auto param35 = model->addOperand(&type3);
  auto param36 = model->addOperand(&type4);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type4);
  auto scoresOut = model->addOperand(&type13);
  auto roiOut = model->addOperand(&type15);
  auto classesOut = model->addOperand(&type14);
  auto batchSplitOut = model->addOperand(&type14);
  auto in8 = model->addOperand(&type2);
  auto param39 = model->addOperand(&type3);
  auto param40 = model->addOperand(&type3);
  auto param41 = model->addOperand(&type4);
  auto param42 = model->addOperand(&type4);
  auto param43 = model->addOperand(&type3);
  auto param44 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type78);
  auto param45 = model->addOperand(&type3);
  auto param46 = model->addOperand(&type3);
  auto out8 = model->addOperand(&type79);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param32_init[] = {0};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static float param33_init[] = {0.3f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static int32_t param34_init[] = {-1};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {0};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static float param36_init[] = {0.4f};
  model->setOperandValue(param36, param36_init, sizeof(float) * 1);
  static float param37_init[] = {1.0f};
  model->setOperandValue(param37, param37_init, sizeof(float) * 1);
  static float param38_init[] = {0.3f};
  model->setOperandValue(param38, param38_init, sizeof(float) * 1);
  static int32_t param39_init[] = {2};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {2};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static float param41_init[] = {2.0f};
  model->setOperandValue(param41, param41_init, sizeof(float) * 1);
  static float param42_init[] = {2.0f};
  model->setOperandValue(param42, param42_init, sizeof(float) * 1);
  static int32_t param43_init[] = {4};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {4};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param45_init[] = {3};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  static int32_t param46_init[] = {3};
  model->setOperandValue(param46, param46_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param32, param33, param34, param35, param36, param37, param38}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in8, roiOut, batchSplitOut, param39, param40, param41, param42, param43, param44, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {featureMap, param45, param46, layout}, {out8});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in8},
    {scoresOut, classesOut, out8});
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
  OperandType type14(Type::TENSOR_INT32, {0});
  OperandType type16(Type::TENSOR_INT32, {1});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type66(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.1f, 128);
  OperandType type68(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type69(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type70(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type71(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  OperandType type80(Type::TENSOR_QUANT8_ASYMM, {0, 1, 2, 2}, 0.1f, 128);
  OperandType type81(Type::TENSOR_QUANT8_ASYMM, {0, 1, 3, 3}, 0.1f, 128);
  // Phase 1, operands
  auto scores = model->addOperand(&type70);
  auto roi = model->addOperand(&type68);
  auto param32 = model->addOperand(&type16);
  auto param33 = model->addOperand(&type4);
  auto param34 = model->addOperand(&type3);
  auto param35 = model->addOperand(&type3);
  auto param36 = model->addOperand(&type4);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type4);
  auto scoresOut = model->addOperand(&type71);
  auto roiOut = model->addOperand(&type69);
  auto classesOut = model->addOperand(&type14);
  auto batchSplitOut = model->addOperand(&type14);
  auto in8 = model->addOperand(&type66);
  auto param39 = model->addOperand(&type3);
  auto param40 = model->addOperand(&type3);
  auto param41 = model->addOperand(&type4);
  auto param42 = model->addOperand(&type4);
  auto param43 = model->addOperand(&type3);
  auto param44 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type80);
  auto param45 = model->addOperand(&type3);
  auto param46 = model->addOperand(&type3);
  auto out8 = model->addOperand(&type81);
  // Phase 2, operations
  static uint8_t scores_init[] = {137, 129};
  model->setOperandValue(scores, scores_init, sizeof(uint8_t) * 2);
  static uint16_t roi_init[] = {8, 8, 80, 80, 0, 0, 80, 80};
  model->setOperandValue(roi, roi_init, sizeof(uint16_t) * 8);
  static int32_t param32_init[] = {0};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static float param33_init[] = {0.3f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static int32_t param34_init[] = {-1};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {0};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static float param36_init[] = {0.4f};
  model->setOperandValue(param36, param36_init, sizeof(float) * 1);
  static float param37_init[] = {1.0f};
  model->setOperandValue(param37, param37_init, sizeof(float) * 1);
  static float param38_init[] = {0.3f};
  model->setOperandValue(param38, param38_init, sizeof(float) * 1);
  static int32_t param39_init[] = {2};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {2};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static float param41_init[] = {2.0f};
  model->setOperandValue(param41, param41_init, sizeof(float) * 1);
  static float param42_init[] = {2.0f};
  model->setOperandValue(param42, param42_init, sizeof(float) * 1);
  static int32_t param43_init[] = {4};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {4};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param45_init[] = {3};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  static int32_t param46_init[] = {3};
  model->setOperandValue(param46, param46_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param32, param33, param34, param35, param36, param37, param38}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in8, roiOut, batchSplitOut, param39, param40, param41, param42, param43, param44, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {featureMap, param45, param46, layout}, {out8});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in8},
    {scoresOut, classesOut, out8});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nchw_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type14(Type::TENSOR_INT32, {0});
  OperandType type16(Type::TENSOR_INT32, {1});
  OperandType type22(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type29(Type::FLOAT16, {});
  OperandType type3(Type::INT32, {});
  OperandType type74(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type75(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type76(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type77(Type::TENSOR_FLOAT16, {0});
  OperandType type82(Type::TENSOR_FLOAT16, {0, 1, 2, 2});
  OperandType type83(Type::TENSOR_FLOAT16, {0, 1, 3, 3});
  // Phase 1, operands
  auto scores = model->addOperand(&type76);
  auto roi = model->addOperand(&type74);
  auto param32 = model->addOperand(&type16);
  auto param33 = model->addOperand(&type29);
  auto param34 = model->addOperand(&type3);
  auto param35 = model->addOperand(&type3);
  auto param36 = model->addOperand(&type29);
  auto param37 = model->addOperand(&type29);
  auto param38 = model->addOperand(&type29);
  auto scoresOut = model->addOperand(&type77);
  auto roiOut = model->addOperand(&type75);
  auto classesOut = model->addOperand(&type14);
  auto batchSplitOut = model->addOperand(&type14);
  auto in8 = model->addOperand(&type22);
  auto param39 = model->addOperand(&type3);
  auto param40 = model->addOperand(&type3);
  auto param41 = model->addOperand(&type29);
  auto param42 = model->addOperand(&type29);
  auto param43 = model->addOperand(&type3);
  auto param44 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type82);
  auto param45 = model->addOperand(&type3);
  auto param46 = model->addOperand(&type3);
  auto out8 = model->addOperand(&type83);
  // Phase 2, operations
  static _Float16 scores_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores, scores_init, sizeof(_Float16) * 2);
  static _Float16 roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(_Float16) * 8);
  static int32_t param32_init[] = {0};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static _Float16 param33_init[] = {0.30000001192092896f};
  model->setOperandValue(param33, param33_init, sizeof(_Float16) * 1);
  static int32_t param34_init[] = {-1};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {0};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static _Float16 param36_init[] = {0.4000000059604645f};
  model->setOperandValue(param36, param36_init, sizeof(_Float16) * 1);
  static _Float16 param37_init[] = {1.0f};
  model->setOperandValue(param37, param37_init, sizeof(_Float16) * 1);
  static _Float16 param38_init[] = {0.30000001192092896f};
  model->setOperandValue(param38, param38_init, sizeof(_Float16) * 1);
  static int32_t param39_init[] = {2};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {2};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static _Float16 param41_init[] = {2.0f};
  model->setOperandValue(param41, param41_init, sizeof(_Float16) * 1);
  static _Float16 param42_init[] = {2.0f};
  model->setOperandValue(param42, param42_init, sizeof(_Float16) * 1);
  static int32_t param43_init[] = {4};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {4};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param45_init[] = {3};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  static int32_t param46_init[] = {3};
  model->setOperandValue(param46, param46_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param32, param33, param34, param35, param36, param37, param38}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in8, roiOut, batchSplitOut, param39, param40, param41, param42, param43, param44, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {featureMap, param45, param46, layout}, {out8});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in8},
    {scoresOut, classesOut, out8});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type12(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type13(Type::TENSOR_FLOAT32, {0});
  OperandType type14(Type::TENSOR_INT32, {0});
  OperandType type15(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type16(Type::TENSOR_INT32, {1});
  OperandType type17(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type11);
  auto roi = model->addOperand(&type12);
  auto param32 = model->addOperand(&type16);
  auto param33 = model->addOperand(&type4);
  auto param34 = model->addOperand(&type3);
  auto param35 = model->addOperand(&type3);
  auto param36 = model->addOperand(&type4);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type4);
  auto scoresOut = model->addOperand(&type13);
  auto roiOut = model->addOperand(&type15);
  auto classesOut = model->addOperand(&type14);
  auto batchSplitOut = model->addOperand(&type14);
  auto in8 = model->addOperand(&type2);
  auto param39 = model->addOperand(&type3);
  auto param40 = model->addOperand(&type3);
  auto param41 = model->addOperand(&type4);
  auto param42 = model->addOperand(&type4);
  auto param43 = model->addOperand(&type3);
  auto param44 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type17);
  auto param45 = model->addOperand(&type3);
  auto param46 = model->addOperand(&type3);
  auto out8 = model->addOperand(&type26);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param32_init[] = {0};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static float param33_init[] = {0.3f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static int32_t param34_init[] = {-1};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {0};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static float param36_init[] = {0.4f};
  model->setOperandValue(param36, param36_init, sizeof(float) * 1);
  static float param37_init[] = {1.0f};
  model->setOperandValue(param37, param37_init, sizeof(float) * 1);
  static float param38_init[] = {0.3f};
  model->setOperandValue(param38, param38_init, sizeof(float) * 1);
  static int32_t param39_init[] = {2};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {2};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static float param41_init[] = {2.0f};
  model->setOperandValue(param41, param41_init, sizeof(float) * 1);
  static float param42_init[] = {2.0f};
  model->setOperandValue(param42, param42_init, sizeof(float) * 1);
  static int32_t param43_init[] = {4};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {4};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param45_init[] = {3};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  static int32_t param46_init[] = {3};
  model->setOperandValue(param46, param46_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param32, param33, param34, param35, param36, param37, param38}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in8, roiOut, batchSplitOut, param39, param40, param41, param42, param43, param44, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {featureMap, param45, param46, layout}, {out8});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in8},
    {scoresOut, classesOut, out8});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type12(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type13(Type::TENSOR_FLOAT32, {0});
  OperandType type14(Type::TENSOR_INT32, {0});
  OperandType type15(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type16(Type::TENSOR_INT32, {1});
  OperandType type17(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type11);
  auto roi = model->addOperand(&type12);
  auto param32 = model->addOperand(&type16);
  auto param33 = model->addOperand(&type4);
  auto param34 = model->addOperand(&type3);
  auto param35 = model->addOperand(&type3);
  auto param36 = model->addOperand(&type4);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type4);
  auto scoresOut = model->addOperand(&type13);
  auto roiOut = model->addOperand(&type15);
  auto classesOut = model->addOperand(&type14);
  auto batchSplitOut = model->addOperand(&type14);
  auto in8 = model->addOperand(&type2);
  auto param39 = model->addOperand(&type3);
  auto param40 = model->addOperand(&type3);
  auto param41 = model->addOperand(&type4);
  auto param42 = model->addOperand(&type4);
  auto param43 = model->addOperand(&type3);
  auto param44 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type17);
  auto param45 = model->addOperand(&type3);
  auto param46 = model->addOperand(&type3);
  auto out8 = model->addOperand(&type26);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param32_init[] = {0};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static float param33_init[] = {0.3f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static int32_t param34_init[] = {-1};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {0};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static float param36_init[] = {0.4f};
  model->setOperandValue(param36, param36_init, sizeof(float) * 1);
  static float param37_init[] = {1.0f};
  model->setOperandValue(param37, param37_init, sizeof(float) * 1);
  static float param38_init[] = {0.3f};
  model->setOperandValue(param38, param38_init, sizeof(float) * 1);
  static int32_t param39_init[] = {2};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {2};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static float param41_init[] = {2.0f};
  model->setOperandValue(param41, param41_init, sizeof(float) * 1);
  static float param42_init[] = {2.0f};
  model->setOperandValue(param42, param42_init, sizeof(float) * 1);
  static int32_t param43_init[] = {4};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {4};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param45_init[] = {3};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  static int32_t param46_init[] = {3};
  model->setOperandValue(param46, param46_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param32, param33, param34, param35, param36, param37, param38}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in8, roiOut, batchSplitOut, param39, param40, param41, param42, param43, param44, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {featureMap, param45, param46, layout}, {out8});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in8},
    {scoresOut, classesOut, out8});
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
  OperandType type14(Type::TENSOR_INT32, {0});
  OperandType type16(Type::TENSOR_INT32, {1});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type65(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 1}, 0.1f, 128);
  OperandType type66(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.1f, 128);
  OperandType type68(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type69(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type70(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type71(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  OperandType type84(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 128);
  // Phase 1, operands
  auto scores = model->addOperand(&type70);
  auto roi = model->addOperand(&type68);
  auto param32 = model->addOperand(&type16);
  auto param33 = model->addOperand(&type4);
  auto param34 = model->addOperand(&type3);
  auto param35 = model->addOperand(&type3);
  auto param36 = model->addOperand(&type4);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type4);
  auto scoresOut = model->addOperand(&type71);
  auto roiOut = model->addOperand(&type69);
  auto classesOut = model->addOperand(&type14);
  auto batchSplitOut = model->addOperand(&type14);
  auto in8 = model->addOperand(&type66);
  auto param39 = model->addOperand(&type3);
  auto param40 = model->addOperand(&type3);
  auto param41 = model->addOperand(&type4);
  auto param42 = model->addOperand(&type4);
  auto param43 = model->addOperand(&type3);
  auto param44 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type65);
  auto param45 = model->addOperand(&type3);
  auto param46 = model->addOperand(&type3);
  auto out8 = model->addOperand(&type84);
  // Phase 2, operations
  static uint8_t scores_init[] = {137, 129};
  model->setOperandValue(scores, scores_init, sizeof(uint8_t) * 2);
  static uint16_t roi_init[] = {8, 8, 80, 80, 0, 0, 80, 80};
  model->setOperandValue(roi, roi_init, sizeof(uint16_t) * 8);
  static int32_t param32_init[] = {0};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static float param33_init[] = {0.3f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static int32_t param34_init[] = {-1};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {0};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static float param36_init[] = {0.4f};
  model->setOperandValue(param36, param36_init, sizeof(float) * 1);
  static float param37_init[] = {1.0f};
  model->setOperandValue(param37, param37_init, sizeof(float) * 1);
  static float param38_init[] = {0.3f};
  model->setOperandValue(param38, param38_init, sizeof(float) * 1);
  static int32_t param39_init[] = {2};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {2};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static float param41_init[] = {2.0f};
  model->setOperandValue(param41, param41_init, sizeof(float) * 1);
  static float param42_init[] = {2.0f};
  model->setOperandValue(param42, param42_init, sizeof(float) * 1);
  static int32_t param43_init[] = {4};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {4};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param45_init[] = {3};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  static int32_t param46_init[] = {3};
  model->setOperandValue(param46, param46_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param32, param33, param34, param35, param36, param37, param38}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in8, roiOut, batchSplitOut, param39, param40, param41, param42, param43, param44, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {featureMap, param45, param46, layout}, {out8});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in8},
    {scoresOut, classesOut, out8});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nhwc_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type14(Type::TENSOR_INT32, {0});
  OperandType type16(Type::TENSOR_INT32, {1});
  OperandType type22(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type29(Type::FLOAT16, {});
  OperandType type3(Type::INT32, {});
  OperandType type72(Type::TENSOR_FLOAT16, {0, 2, 2, 1});
  OperandType type74(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type75(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type76(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type85(Type::TENSOR_FLOAT16, {0});
  // Phase 1, operands
  auto scores = model->addOperand(&type76);
  auto roi = model->addOperand(&type74);
  auto param32 = model->addOperand(&type16);
  auto param33 = model->addOperand(&type29);
  auto param34 = model->addOperand(&type3);
  auto param35 = model->addOperand(&type3);
  auto param36 = model->addOperand(&type29);
  auto param37 = model->addOperand(&type29);
  auto param38 = model->addOperand(&type29);
  auto scoresOut = model->addOperand(&type85);
  auto roiOut = model->addOperand(&type75);
  auto classesOut = model->addOperand(&type14);
  auto batchSplitOut = model->addOperand(&type14);
  auto in8 = model->addOperand(&type22);
  auto param39 = model->addOperand(&type3);
  auto param40 = model->addOperand(&type3);
  auto param41 = model->addOperand(&type29);
  auto param42 = model->addOperand(&type29);
  auto param43 = model->addOperand(&type3);
  auto param44 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type72);
  auto param45 = model->addOperand(&type3);
  auto param46 = model->addOperand(&type3);
  auto out8 = model->addOperand(&type28);
  // Phase 2, operations
  static _Float16 scores_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores, scores_init, sizeof(_Float16) * 2);
  static _Float16 roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(_Float16) * 8);
  static int32_t param32_init[] = {0};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static _Float16 param33_init[] = {0.30000001192092896f};
  model->setOperandValue(param33, param33_init, sizeof(_Float16) * 1);
  static int32_t param34_init[] = {-1};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {0};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static _Float16 param36_init[] = {0.4000000059604645f};
  model->setOperandValue(param36, param36_init, sizeof(_Float16) * 1);
  static _Float16 param37_init[] = {1.0f};
  model->setOperandValue(param37, param37_init, sizeof(_Float16) * 1);
  static _Float16 param38_init[] = {0.30000001192092896f};
  model->setOperandValue(param38, param38_init, sizeof(_Float16) * 1);
  static int32_t param39_init[] = {2};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {2};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static _Float16 param41_init[] = {2.0f};
  model->setOperandValue(param41, param41_init, sizeof(_Float16) * 1);
  static _Float16 param42_init[] = {2.0f};
  model->setOperandValue(param42, param42_init, sizeof(_Float16) * 1);
  static int32_t param43_init[] = {4};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {4};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param45_init[] = {3};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  static int32_t param46_init[] = {3};
  model->setOperandValue(param46, param46_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param32, param33, param34, param35, param36, param37, param38}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in8, roiOut, batchSplitOut, param39, param40, param41, param42, param43, param44, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {featureMap, param45, param46, layout}, {out8});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in8},
    {scoresOut, classesOut, out8});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type12(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type13(Type::TENSOR_FLOAT32, {0});
  OperandType type14(Type::TENSOR_INT32, {0});
  OperandType type15(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type16(Type::TENSOR_INT32, {1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type78(Type::TENSOR_FLOAT32, {0, 1, 2, 2});
  // Phase 1, operands
  auto scores = model->addOperand(&type11);
  auto roi = model->addOperand(&type12);
  auto param32 = model->addOperand(&type16);
  auto param33 = model->addOperand(&type4);
  auto param34 = model->addOperand(&type3);
  auto param35 = model->addOperand(&type3);
  auto param36 = model->addOperand(&type4);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type4);
  auto scoresOut = model->addOperand(&type13);
  auto roiOut = model->addOperand(&type15);
  auto classesOut = model->addOperand(&type14);
  auto batchSplitOut = model->addOperand(&type14);
  auto in8 = model->addOperand(&type2);
  auto param39 = model->addOperand(&type3);
  auto param40 = model->addOperand(&type3);
  auto param41 = model->addOperand(&type4);
  auto param42 = model->addOperand(&type4);
  auto param43 = model->addOperand(&type3);
  auto param44 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type78);
  auto param45 = model->addOperand(&type3);
  auto param46 = model->addOperand(&type3);
  auto out8 = model->addOperand(&type26);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param32_init[] = {0};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static float param33_init[] = {0.3f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static int32_t param34_init[] = {-1};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {0};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static float param36_init[] = {0.4f};
  model->setOperandValue(param36, param36_init, sizeof(float) * 1);
  static float param37_init[] = {1.0f};
  model->setOperandValue(param37, param37_init, sizeof(float) * 1);
  static float param38_init[] = {0.3f};
  model->setOperandValue(param38, param38_init, sizeof(float) * 1);
  static int32_t param39_init[] = {2};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {2};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static float param41_init[] = {2.0f};
  model->setOperandValue(param41, param41_init, sizeof(float) * 1);
  static float param42_init[] = {2.0f};
  model->setOperandValue(param42, param42_init, sizeof(float) * 1);
  static int32_t param43_init[] = {4};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {4};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param45_init[] = {3};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  static int32_t param46_init[] = {3};
  model->setOperandValue(param46, param46_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param32, param33, param34, param35, param36, param37, param38}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in8, roiOut, batchSplitOut, param39, param40, param41, param42, param43, param44, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {featureMap, param45, param46, layout}, {out8});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in8},
    {scoresOut, classesOut, out8});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type12(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type13(Type::TENSOR_FLOAT32, {0});
  OperandType type14(Type::TENSOR_INT32, {0});
  OperandType type15(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type16(Type::TENSOR_INT32, {1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type78(Type::TENSOR_FLOAT32, {0, 1, 2, 2});
  // Phase 1, operands
  auto scores = model->addOperand(&type11);
  auto roi = model->addOperand(&type12);
  auto param32 = model->addOperand(&type16);
  auto param33 = model->addOperand(&type4);
  auto param34 = model->addOperand(&type3);
  auto param35 = model->addOperand(&type3);
  auto param36 = model->addOperand(&type4);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type4);
  auto scoresOut = model->addOperand(&type13);
  auto roiOut = model->addOperand(&type15);
  auto classesOut = model->addOperand(&type14);
  auto batchSplitOut = model->addOperand(&type14);
  auto in8 = model->addOperand(&type2);
  auto param39 = model->addOperand(&type3);
  auto param40 = model->addOperand(&type3);
  auto param41 = model->addOperand(&type4);
  auto param42 = model->addOperand(&type4);
  auto param43 = model->addOperand(&type3);
  auto param44 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type78);
  auto param45 = model->addOperand(&type3);
  auto param46 = model->addOperand(&type3);
  auto out8 = model->addOperand(&type26);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param32_init[] = {0};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static float param33_init[] = {0.3f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static int32_t param34_init[] = {-1};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {0};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static float param36_init[] = {0.4f};
  model->setOperandValue(param36, param36_init, sizeof(float) * 1);
  static float param37_init[] = {1.0f};
  model->setOperandValue(param37, param37_init, sizeof(float) * 1);
  static float param38_init[] = {0.3f};
  model->setOperandValue(param38, param38_init, sizeof(float) * 1);
  static int32_t param39_init[] = {2};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {2};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static float param41_init[] = {2.0f};
  model->setOperandValue(param41, param41_init, sizeof(float) * 1);
  static float param42_init[] = {2.0f};
  model->setOperandValue(param42, param42_init, sizeof(float) * 1);
  static int32_t param43_init[] = {4};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {4};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param45_init[] = {3};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  static int32_t param46_init[] = {3};
  model->setOperandValue(param46, param46_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param32, param33, param34, param35, param36, param37, param38}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in8, roiOut, batchSplitOut, param39, param40, param41, param42, param43, param44, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {featureMap, param45, param46, layout}, {out8});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in8},
    {scoresOut, classesOut, out8});
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
  OperandType type14(Type::TENSOR_INT32, {0});
  OperandType type16(Type::TENSOR_INT32, {1});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type66(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.1f, 128);
  OperandType type68(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type69(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type70(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type71(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  OperandType type80(Type::TENSOR_QUANT8_ASYMM, {0, 1, 2, 2}, 0.1f, 128);
  OperandType type84(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 128);
  // Phase 1, operands
  auto scores = model->addOperand(&type70);
  auto roi = model->addOperand(&type68);
  auto param32 = model->addOperand(&type16);
  auto param33 = model->addOperand(&type4);
  auto param34 = model->addOperand(&type3);
  auto param35 = model->addOperand(&type3);
  auto param36 = model->addOperand(&type4);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type4);
  auto scoresOut = model->addOperand(&type71);
  auto roiOut = model->addOperand(&type69);
  auto classesOut = model->addOperand(&type14);
  auto batchSplitOut = model->addOperand(&type14);
  auto in8 = model->addOperand(&type66);
  auto param39 = model->addOperand(&type3);
  auto param40 = model->addOperand(&type3);
  auto param41 = model->addOperand(&type4);
  auto param42 = model->addOperand(&type4);
  auto param43 = model->addOperand(&type3);
  auto param44 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type80);
  auto param45 = model->addOperand(&type3);
  auto param46 = model->addOperand(&type3);
  auto out8 = model->addOperand(&type84);
  // Phase 2, operations
  static uint8_t scores_init[] = {137, 129};
  model->setOperandValue(scores, scores_init, sizeof(uint8_t) * 2);
  static uint16_t roi_init[] = {8, 8, 80, 80, 0, 0, 80, 80};
  model->setOperandValue(roi, roi_init, sizeof(uint16_t) * 8);
  static int32_t param32_init[] = {0};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static float param33_init[] = {0.3f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static int32_t param34_init[] = {-1};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {0};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static float param36_init[] = {0.4f};
  model->setOperandValue(param36, param36_init, sizeof(float) * 1);
  static float param37_init[] = {1.0f};
  model->setOperandValue(param37, param37_init, sizeof(float) * 1);
  static float param38_init[] = {0.3f};
  model->setOperandValue(param38, param38_init, sizeof(float) * 1);
  static int32_t param39_init[] = {2};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {2};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static float param41_init[] = {2.0f};
  model->setOperandValue(param41, param41_init, sizeof(float) * 1);
  static float param42_init[] = {2.0f};
  model->setOperandValue(param42, param42_init, sizeof(float) * 1);
  static int32_t param43_init[] = {4};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {4};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param45_init[] = {3};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  static int32_t param46_init[] = {3};
  model->setOperandValue(param46, param46_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param32, param33, param34, param35, param36, param37, param38}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in8, roiOut, batchSplitOut, param39, param40, param41, param42, param43, param44, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {featureMap, param45, param46, layout}, {out8});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in8},
    {scoresOut, classesOut, out8});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nchw_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type14(Type::TENSOR_INT32, {0});
  OperandType type16(Type::TENSOR_INT32, {1});
  OperandType type22(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type29(Type::FLOAT16, {});
  OperandType type3(Type::INT32, {});
  OperandType type74(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type75(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type76(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type82(Type::TENSOR_FLOAT16, {0, 1, 2, 2});
  OperandType type85(Type::TENSOR_FLOAT16, {0});
  // Phase 1, operands
  auto scores = model->addOperand(&type76);
  auto roi = model->addOperand(&type74);
  auto param32 = model->addOperand(&type16);
  auto param33 = model->addOperand(&type29);
  auto param34 = model->addOperand(&type3);
  auto param35 = model->addOperand(&type3);
  auto param36 = model->addOperand(&type29);
  auto param37 = model->addOperand(&type29);
  auto param38 = model->addOperand(&type29);
  auto scoresOut = model->addOperand(&type85);
  auto roiOut = model->addOperand(&type75);
  auto classesOut = model->addOperand(&type14);
  auto batchSplitOut = model->addOperand(&type14);
  auto in8 = model->addOperand(&type22);
  auto param39 = model->addOperand(&type3);
  auto param40 = model->addOperand(&type3);
  auto param41 = model->addOperand(&type29);
  auto param42 = model->addOperand(&type29);
  auto param43 = model->addOperand(&type3);
  auto param44 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type82);
  auto param45 = model->addOperand(&type3);
  auto param46 = model->addOperand(&type3);
  auto out8 = model->addOperand(&type28);
  // Phase 2, operations
  static _Float16 scores_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores, scores_init, sizeof(_Float16) * 2);
  static _Float16 roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(_Float16) * 8);
  static int32_t param32_init[] = {0};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static _Float16 param33_init[] = {0.30000001192092896f};
  model->setOperandValue(param33, param33_init, sizeof(_Float16) * 1);
  static int32_t param34_init[] = {-1};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {0};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static _Float16 param36_init[] = {0.4000000059604645f};
  model->setOperandValue(param36, param36_init, sizeof(_Float16) * 1);
  static _Float16 param37_init[] = {1.0f};
  model->setOperandValue(param37, param37_init, sizeof(_Float16) * 1);
  static _Float16 param38_init[] = {0.30000001192092896f};
  model->setOperandValue(param38, param38_init, sizeof(_Float16) * 1);
  static int32_t param39_init[] = {2};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {2};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static _Float16 param41_init[] = {2.0f};
  model->setOperandValue(param41, param41_init, sizeof(_Float16) * 1);
  static _Float16 param42_init[] = {2.0f};
  model->setOperandValue(param42, param42_init, sizeof(_Float16) * 1);
  static int32_t param43_init[] = {4};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {4};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param45_init[] = {3};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  static int32_t param46_init[] = {3};
  model->setOperandValue(param46, param46_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param32, param33, param34, param35, param36, param37, param38}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in8, roiOut, batchSplitOut, param39, param40, param41, param42, param43, param44, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {featureMap, param45, param46, layout}, {out8});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in8},
    {scoresOut, classesOut, out8});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nhwc_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type12(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type13(Type::TENSOR_FLOAT32, {0});
  OperandType type14(Type::TENSOR_INT32, {0});
  OperandType type15(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type16(Type::TENSOR_INT32, {1});
  OperandType type17(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type18(Type::TENSOR_FLOAT32, {0, 3, 3, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type11);
  auto roi1 = model->addOperand(&type12);
  auto param47 = model->addOperand(&type16);
  auto param48 = model->addOperand(&type4);
  auto param49 = model->addOperand(&type3);
  auto param50 = model->addOperand(&type3);
  auto param51 = model->addOperand(&type4);
  auto param52 = model->addOperand(&type4);
  auto param53 = model->addOperand(&type4);
  auto scoresOut1 = model->addOperand(&type13);
  auto roiOut1 = model->addOperand(&type15);
  auto classesOut1 = model->addOperand(&type14);
  auto batchSplitOut1 = model->addOperand(&type14);
  auto in9 = model->addOperand(&type2);
  auto param54 = model->addOperand(&type3);
  auto param55 = model->addOperand(&type3);
  auto param56 = model->addOperand(&type4);
  auto param57 = model->addOperand(&type4);
  auto param58 = model->addOperand(&type3);
  auto param59 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type17);
  auto param60 = model->addOperand(&type4);
  auto param61 = model->addOperand(&type4);
  auto out9 = model->addOperand(&type18);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param47_init[] = {0};
  model->setOperandValue(param47, param47_init, sizeof(int32_t) * 1);
  static float param48_init[] = {0.3f};
  model->setOperandValue(param48, param48_init, sizeof(float) * 1);
  static int32_t param49_init[] = {-1};
  model->setOperandValue(param49, param49_init, sizeof(int32_t) * 1);
  static int32_t param50_init[] = {0};
  model->setOperandValue(param50, param50_init, sizeof(int32_t) * 1);
  static float param51_init[] = {0.4f};
  model->setOperandValue(param51, param51_init, sizeof(float) * 1);
  static float param52_init[] = {1.0f};
  model->setOperandValue(param52, param52_init, sizeof(float) * 1);
  static float param53_init[] = {0.3f};
  model->setOperandValue(param53, param53_init, sizeof(float) * 1);
  static int32_t param54_init[] = {2};
  model->setOperandValue(param54, param54_init, sizeof(int32_t) * 1);
  static int32_t param55_init[] = {2};
  model->setOperandValue(param55, param55_init, sizeof(int32_t) * 1);
  static float param56_init[] = {2.0f};
  model->setOperandValue(param56, param56_init, sizeof(float) * 1);
  static float param57_init[] = {2.0f};
  model->setOperandValue(param57, param57_init, sizeof(float) * 1);
  static int32_t param58_init[] = {4};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static int32_t param59_init[] = {4};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float param60_init[] = {1.6f};
  model->setOperandValue(param60, param60_init, sizeof(float) * 1);
  static float param61_init[] = {1.6f};
  model->setOperandValue(param61, param61_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param47, param48, param49, param50, param51, param52, param53}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in9, roiOut1, batchSplitOut1, param54, param55, param56, param57, param58, param59, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {featureMap1, param60, param61, layout}, {out9});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in9},
    {scoresOut1, classesOut1, out9});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nhwc_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nhwc_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type12(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type13(Type::TENSOR_FLOAT32, {0});
  OperandType type14(Type::TENSOR_INT32, {0});
  OperandType type15(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type16(Type::TENSOR_INT32, {1});
  OperandType type17(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type18(Type::TENSOR_FLOAT32, {0, 3, 3, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type11);
  auto roi1 = model->addOperand(&type12);
  auto param47 = model->addOperand(&type16);
  auto param48 = model->addOperand(&type4);
  auto param49 = model->addOperand(&type3);
  auto param50 = model->addOperand(&type3);
  auto param51 = model->addOperand(&type4);
  auto param52 = model->addOperand(&type4);
  auto param53 = model->addOperand(&type4);
  auto scoresOut1 = model->addOperand(&type13);
  auto roiOut1 = model->addOperand(&type15);
  auto classesOut1 = model->addOperand(&type14);
  auto batchSplitOut1 = model->addOperand(&type14);
  auto in9 = model->addOperand(&type2);
  auto param54 = model->addOperand(&type3);
  auto param55 = model->addOperand(&type3);
  auto param56 = model->addOperand(&type4);
  auto param57 = model->addOperand(&type4);
  auto param58 = model->addOperand(&type3);
  auto param59 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type17);
  auto param60 = model->addOperand(&type4);
  auto param61 = model->addOperand(&type4);
  auto out9 = model->addOperand(&type18);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param47_init[] = {0};
  model->setOperandValue(param47, param47_init, sizeof(int32_t) * 1);
  static float param48_init[] = {0.3f};
  model->setOperandValue(param48, param48_init, sizeof(float) * 1);
  static int32_t param49_init[] = {-1};
  model->setOperandValue(param49, param49_init, sizeof(int32_t) * 1);
  static int32_t param50_init[] = {0};
  model->setOperandValue(param50, param50_init, sizeof(int32_t) * 1);
  static float param51_init[] = {0.4f};
  model->setOperandValue(param51, param51_init, sizeof(float) * 1);
  static float param52_init[] = {1.0f};
  model->setOperandValue(param52, param52_init, sizeof(float) * 1);
  static float param53_init[] = {0.3f};
  model->setOperandValue(param53, param53_init, sizeof(float) * 1);
  static int32_t param54_init[] = {2};
  model->setOperandValue(param54, param54_init, sizeof(int32_t) * 1);
  static int32_t param55_init[] = {2};
  model->setOperandValue(param55, param55_init, sizeof(int32_t) * 1);
  static float param56_init[] = {2.0f};
  model->setOperandValue(param56, param56_init, sizeof(float) * 1);
  static float param57_init[] = {2.0f};
  model->setOperandValue(param57, param57_init, sizeof(float) * 1);
  static int32_t param58_init[] = {4};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static int32_t param59_init[] = {4};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float param60_init[] = {1.6f};
  model->setOperandValue(param60, param60_init, sizeof(float) * 1);
  static float param61_init[] = {1.6f};
  model->setOperandValue(param61, param61_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param47, param48, param49, param50, param51, param52, param53}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in9, roiOut1, batchSplitOut1, param54, param55, param56, param57, param58, param59, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {featureMap1, param60, param61, layout}, {out9});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in9},
    {scoresOut1, classesOut1, out9});
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
  OperandType type14(Type::TENSOR_INT32, {0});
  OperandType type16(Type::TENSOR_INT32, {1});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type65(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 1}, 0.1f, 128);
  OperandType type66(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.1f, 128);
  OperandType type67(Type::TENSOR_QUANT8_ASYMM, {0, 3, 3, 1}, 0.1f, 128);
  OperandType type68(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type69(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type70(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type71(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  // Phase 1, operands
  auto scores1 = model->addOperand(&type70);
  auto roi1 = model->addOperand(&type68);
  auto param47 = model->addOperand(&type16);
  auto param48 = model->addOperand(&type4);
  auto param49 = model->addOperand(&type3);
  auto param50 = model->addOperand(&type3);
  auto param51 = model->addOperand(&type4);
  auto param52 = model->addOperand(&type4);
  auto param53 = model->addOperand(&type4);
  auto scoresOut1 = model->addOperand(&type71);
  auto roiOut1 = model->addOperand(&type69);
  auto classesOut1 = model->addOperand(&type14);
  auto batchSplitOut1 = model->addOperand(&type14);
  auto in9 = model->addOperand(&type66);
  auto param54 = model->addOperand(&type3);
  auto param55 = model->addOperand(&type3);
  auto param56 = model->addOperand(&type4);
  auto param57 = model->addOperand(&type4);
  auto param58 = model->addOperand(&type3);
  auto param59 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type65);
  auto param60 = model->addOperand(&type4);
  auto param61 = model->addOperand(&type4);
  auto out9 = model->addOperand(&type67);
  // Phase 2, operations
  static uint8_t scores1_init[] = {137, 129};
  model->setOperandValue(scores1, scores1_init, sizeof(uint8_t) * 2);
  static uint16_t roi1_init[] = {8, 8, 80, 80, 0, 0, 80, 80};
  model->setOperandValue(roi1, roi1_init, sizeof(uint16_t) * 8);
  static int32_t param47_init[] = {0};
  model->setOperandValue(param47, param47_init, sizeof(int32_t) * 1);
  static float param48_init[] = {0.3f};
  model->setOperandValue(param48, param48_init, sizeof(float) * 1);
  static int32_t param49_init[] = {-1};
  model->setOperandValue(param49, param49_init, sizeof(int32_t) * 1);
  static int32_t param50_init[] = {0};
  model->setOperandValue(param50, param50_init, sizeof(int32_t) * 1);
  static float param51_init[] = {0.4f};
  model->setOperandValue(param51, param51_init, sizeof(float) * 1);
  static float param52_init[] = {1.0f};
  model->setOperandValue(param52, param52_init, sizeof(float) * 1);
  static float param53_init[] = {0.3f};
  model->setOperandValue(param53, param53_init, sizeof(float) * 1);
  static int32_t param54_init[] = {2};
  model->setOperandValue(param54, param54_init, sizeof(int32_t) * 1);
  static int32_t param55_init[] = {2};
  model->setOperandValue(param55, param55_init, sizeof(int32_t) * 1);
  static float param56_init[] = {2.0f};
  model->setOperandValue(param56, param56_init, sizeof(float) * 1);
  static float param57_init[] = {2.0f};
  model->setOperandValue(param57, param57_init, sizeof(float) * 1);
  static int32_t param58_init[] = {4};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static int32_t param59_init[] = {4};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float param60_init[] = {1.6f};
  model->setOperandValue(param60, param60_init, sizeof(float) * 1);
  static float param61_init[] = {1.6f};
  model->setOperandValue(param61, param61_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param47, param48, param49, param50, param51, param52, param53}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in9, roiOut1, batchSplitOut1, param54, param55, param56, param57, param58, param59, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {featureMap1, param60, param61, layout}, {out9});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in9},
    {scoresOut1, classesOut1, out9});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nhwc_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nhwc_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type14(Type::TENSOR_INT32, {0});
  OperandType type16(Type::TENSOR_INT32, {1});
  OperandType type22(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type29(Type::FLOAT16, {});
  OperandType type3(Type::INT32, {});
  OperandType type72(Type::TENSOR_FLOAT16, {0, 2, 2, 1});
  OperandType type73(Type::TENSOR_FLOAT16, {0, 3, 3, 1});
  OperandType type74(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type75(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type76(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type77(Type::TENSOR_FLOAT16, {0});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type76);
  auto roi1 = model->addOperand(&type74);
  auto param47 = model->addOperand(&type16);
  auto param48 = model->addOperand(&type29);
  auto param49 = model->addOperand(&type3);
  auto param50 = model->addOperand(&type3);
  auto param51 = model->addOperand(&type29);
  auto param52 = model->addOperand(&type29);
  auto param53 = model->addOperand(&type29);
  auto scoresOut1 = model->addOperand(&type77);
  auto roiOut1 = model->addOperand(&type75);
  auto classesOut1 = model->addOperand(&type14);
  auto batchSplitOut1 = model->addOperand(&type14);
  auto in9 = model->addOperand(&type22);
  auto param54 = model->addOperand(&type3);
  auto param55 = model->addOperand(&type3);
  auto param56 = model->addOperand(&type29);
  auto param57 = model->addOperand(&type29);
  auto param58 = model->addOperand(&type3);
  auto param59 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type72);
  auto param60 = model->addOperand(&type29);
  auto param61 = model->addOperand(&type29);
  auto out9 = model->addOperand(&type73);
  // Phase 2, operations
  static _Float16 scores1_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores1, scores1_init, sizeof(_Float16) * 2);
  static _Float16 roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(_Float16) * 8);
  static int32_t param47_init[] = {0};
  model->setOperandValue(param47, param47_init, sizeof(int32_t) * 1);
  static _Float16 param48_init[] = {0.30000001192092896f};
  model->setOperandValue(param48, param48_init, sizeof(_Float16) * 1);
  static int32_t param49_init[] = {-1};
  model->setOperandValue(param49, param49_init, sizeof(int32_t) * 1);
  static int32_t param50_init[] = {0};
  model->setOperandValue(param50, param50_init, sizeof(int32_t) * 1);
  static _Float16 param51_init[] = {0.4000000059604645f};
  model->setOperandValue(param51, param51_init, sizeof(_Float16) * 1);
  static _Float16 param52_init[] = {1.0f};
  model->setOperandValue(param52, param52_init, sizeof(_Float16) * 1);
  static _Float16 param53_init[] = {0.30000001192092896f};
  model->setOperandValue(param53, param53_init, sizeof(_Float16) * 1);
  static int32_t param54_init[] = {2};
  model->setOperandValue(param54, param54_init, sizeof(int32_t) * 1);
  static int32_t param55_init[] = {2};
  model->setOperandValue(param55, param55_init, sizeof(int32_t) * 1);
  static _Float16 param56_init[] = {2.0f};
  model->setOperandValue(param56, param56_init, sizeof(_Float16) * 1);
  static _Float16 param57_init[] = {2.0f};
  model->setOperandValue(param57, param57_init, sizeof(_Float16) * 1);
  static int32_t param58_init[] = {4};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static int32_t param59_init[] = {4};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static _Float16 param60_init[] = {1.600000023841858f};
  model->setOperandValue(param60, param60_init, sizeof(_Float16) * 1);
  static _Float16 param61_init[] = {1.600000023841858f};
  model->setOperandValue(param61, param61_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param47, param48, param49, param50, param51, param52, param53}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in9, roiOut1, batchSplitOut1, param54, param55, param56, param57, param58, param59, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {featureMap1, param60, param61, layout}, {out9});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in9},
    {scoresOut1, classesOut1, out9});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nhwc_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nchw_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type12(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type13(Type::TENSOR_FLOAT32, {0});
  OperandType type14(Type::TENSOR_INT32, {0});
  OperandType type15(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type16(Type::TENSOR_INT32, {1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type78(Type::TENSOR_FLOAT32, {0, 1, 2, 2});
  OperandType type79(Type::TENSOR_FLOAT32, {0, 1, 3, 3});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type11);
  auto roi1 = model->addOperand(&type12);
  auto param47 = model->addOperand(&type16);
  auto param48 = model->addOperand(&type4);
  auto param49 = model->addOperand(&type3);
  auto param50 = model->addOperand(&type3);
  auto param51 = model->addOperand(&type4);
  auto param52 = model->addOperand(&type4);
  auto param53 = model->addOperand(&type4);
  auto scoresOut1 = model->addOperand(&type13);
  auto roiOut1 = model->addOperand(&type15);
  auto classesOut1 = model->addOperand(&type14);
  auto batchSplitOut1 = model->addOperand(&type14);
  auto in9 = model->addOperand(&type2);
  auto param54 = model->addOperand(&type3);
  auto param55 = model->addOperand(&type3);
  auto param56 = model->addOperand(&type4);
  auto param57 = model->addOperand(&type4);
  auto param58 = model->addOperand(&type3);
  auto param59 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type78);
  auto param60 = model->addOperand(&type4);
  auto param61 = model->addOperand(&type4);
  auto out9 = model->addOperand(&type79);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param47_init[] = {0};
  model->setOperandValue(param47, param47_init, sizeof(int32_t) * 1);
  static float param48_init[] = {0.3f};
  model->setOperandValue(param48, param48_init, sizeof(float) * 1);
  static int32_t param49_init[] = {-1};
  model->setOperandValue(param49, param49_init, sizeof(int32_t) * 1);
  static int32_t param50_init[] = {0};
  model->setOperandValue(param50, param50_init, sizeof(int32_t) * 1);
  static float param51_init[] = {0.4f};
  model->setOperandValue(param51, param51_init, sizeof(float) * 1);
  static float param52_init[] = {1.0f};
  model->setOperandValue(param52, param52_init, sizeof(float) * 1);
  static float param53_init[] = {0.3f};
  model->setOperandValue(param53, param53_init, sizeof(float) * 1);
  static int32_t param54_init[] = {2};
  model->setOperandValue(param54, param54_init, sizeof(int32_t) * 1);
  static int32_t param55_init[] = {2};
  model->setOperandValue(param55, param55_init, sizeof(int32_t) * 1);
  static float param56_init[] = {2.0f};
  model->setOperandValue(param56, param56_init, sizeof(float) * 1);
  static float param57_init[] = {2.0f};
  model->setOperandValue(param57, param57_init, sizeof(float) * 1);
  static int32_t param58_init[] = {4};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static int32_t param59_init[] = {4};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float param60_init[] = {1.6f};
  model->setOperandValue(param60, param60_init, sizeof(float) * 1);
  static float param61_init[] = {1.6f};
  model->setOperandValue(param61, param61_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param47, param48, param49, param50, param51, param52, param53}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in9, roiOut1, batchSplitOut1, param54, param55, param56, param57, param58, param59, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {featureMap1, param60, param61, layout}, {out9});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in9},
    {scoresOut1, classesOut1, out9});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nchw_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nchw_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type12(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type13(Type::TENSOR_FLOAT32, {0});
  OperandType type14(Type::TENSOR_INT32, {0});
  OperandType type15(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type16(Type::TENSOR_INT32, {1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type78(Type::TENSOR_FLOAT32, {0, 1, 2, 2});
  OperandType type79(Type::TENSOR_FLOAT32, {0, 1, 3, 3});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type11);
  auto roi1 = model->addOperand(&type12);
  auto param47 = model->addOperand(&type16);
  auto param48 = model->addOperand(&type4);
  auto param49 = model->addOperand(&type3);
  auto param50 = model->addOperand(&type3);
  auto param51 = model->addOperand(&type4);
  auto param52 = model->addOperand(&type4);
  auto param53 = model->addOperand(&type4);
  auto scoresOut1 = model->addOperand(&type13);
  auto roiOut1 = model->addOperand(&type15);
  auto classesOut1 = model->addOperand(&type14);
  auto batchSplitOut1 = model->addOperand(&type14);
  auto in9 = model->addOperand(&type2);
  auto param54 = model->addOperand(&type3);
  auto param55 = model->addOperand(&type3);
  auto param56 = model->addOperand(&type4);
  auto param57 = model->addOperand(&type4);
  auto param58 = model->addOperand(&type3);
  auto param59 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type78);
  auto param60 = model->addOperand(&type4);
  auto param61 = model->addOperand(&type4);
  auto out9 = model->addOperand(&type79);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param47_init[] = {0};
  model->setOperandValue(param47, param47_init, sizeof(int32_t) * 1);
  static float param48_init[] = {0.3f};
  model->setOperandValue(param48, param48_init, sizeof(float) * 1);
  static int32_t param49_init[] = {-1};
  model->setOperandValue(param49, param49_init, sizeof(int32_t) * 1);
  static int32_t param50_init[] = {0};
  model->setOperandValue(param50, param50_init, sizeof(int32_t) * 1);
  static float param51_init[] = {0.4f};
  model->setOperandValue(param51, param51_init, sizeof(float) * 1);
  static float param52_init[] = {1.0f};
  model->setOperandValue(param52, param52_init, sizeof(float) * 1);
  static float param53_init[] = {0.3f};
  model->setOperandValue(param53, param53_init, sizeof(float) * 1);
  static int32_t param54_init[] = {2};
  model->setOperandValue(param54, param54_init, sizeof(int32_t) * 1);
  static int32_t param55_init[] = {2};
  model->setOperandValue(param55, param55_init, sizeof(int32_t) * 1);
  static float param56_init[] = {2.0f};
  model->setOperandValue(param56, param56_init, sizeof(float) * 1);
  static float param57_init[] = {2.0f};
  model->setOperandValue(param57, param57_init, sizeof(float) * 1);
  static int32_t param58_init[] = {4};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static int32_t param59_init[] = {4};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float param60_init[] = {1.6f};
  model->setOperandValue(param60, param60_init, sizeof(float) * 1);
  static float param61_init[] = {1.6f};
  model->setOperandValue(param61, param61_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param47, param48, param49, param50, param51, param52, param53}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in9, roiOut1, batchSplitOut1, param54, param55, param56, param57, param58, param59, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {featureMap1, param60, param61, layout}, {out9});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in9},
    {scoresOut1, classesOut1, out9});
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
  OperandType type14(Type::TENSOR_INT32, {0});
  OperandType type16(Type::TENSOR_INT32, {1});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type66(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.1f, 128);
  OperandType type68(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type69(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type70(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type71(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  OperandType type80(Type::TENSOR_QUANT8_ASYMM, {0, 1, 2, 2}, 0.1f, 128);
  OperandType type81(Type::TENSOR_QUANT8_ASYMM, {0, 1, 3, 3}, 0.1f, 128);
  // Phase 1, operands
  auto scores1 = model->addOperand(&type70);
  auto roi1 = model->addOperand(&type68);
  auto param47 = model->addOperand(&type16);
  auto param48 = model->addOperand(&type4);
  auto param49 = model->addOperand(&type3);
  auto param50 = model->addOperand(&type3);
  auto param51 = model->addOperand(&type4);
  auto param52 = model->addOperand(&type4);
  auto param53 = model->addOperand(&type4);
  auto scoresOut1 = model->addOperand(&type71);
  auto roiOut1 = model->addOperand(&type69);
  auto classesOut1 = model->addOperand(&type14);
  auto batchSplitOut1 = model->addOperand(&type14);
  auto in9 = model->addOperand(&type66);
  auto param54 = model->addOperand(&type3);
  auto param55 = model->addOperand(&type3);
  auto param56 = model->addOperand(&type4);
  auto param57 = model->addOperand(&type4);
  auto param58 = model->addOperand(&type3);
  auto param59 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type80);
  auto param60 = model->addOperand(&type4);
  auto param61 = model->addOperand(&type4);
  auto out9 = model->addOperand(&type81);
  // Phase 2, operations
  static uint8_t scores1_init[] = {137, 129};
  model->setOperandValue(scores1, scores1_init, sizeof(uint8_t) * 2);
  static uint16_t roi1_init[] = {8, 8, 80, 80, 0, 0, 80, 80};
  model->setOperandValue(roi1, roi1_init, sizeof(uint16_t) * 8);
  static int32_t param47_init[] = {0};
  model->setOperandValue(param47, param47_init, sizeof(int32_t) * 1);
  static float param48_init[] = {0.3f};
  model->setOperandValue(param48, param48_init, sizeof(float) * 1);
  static int32_t param49_init[] = {-1};
  model->setOperandValue(param49, param49_init, sizeof(int32_t) * 1);
  static int32_t param50_init[] = {0};
  model->setOperandValue(param50, param50_init, sizeof(int32_t) * 1);
  static float param51_init[] = {0.4f};
  model->setOperandValue(param51, param51_init, sizeof(float) * 1);
  static float param52_init[] = {1.0f};
  model->setOperandValue(param52, param52_init, sizeof(float) * 1);
  static float param53_init[] = {0.3f};
  model->setOperandValue(param53, param53_init, sizeof(float) * 1);
  static int32_t param54_init[] = {2};
  model->setOperandValue(param54, param54_init, sizeof(int32_t) * 1);
  static int32_t param55_init[] = {2};
  model->setOperandValue(param55, param55_init, sizeof(int32_t) * 1);
  static float param56_init[] = {2.0f};
  model->setOperandValue(param56, param56_init, sizeof(float) * 1);
  static float param57_init[] = {2.0f};
  model->setOperandValue(param57, param57_init, sizeof(float) * 1);
  static int32_t param58_init[] = {4};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static int32_t param59_init[] = {4};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float param60_init[] = {1.6f};
  model->setOperandValue(param60, param60_init, sizeof(float) * 1);
  static float param61_init[] = {1.6f};
  model->setOperandValue(param61, param61_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param47, param48, param49, param50, param51, param52, param53}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in9, roiOut1, batchSplitOut1, param54, param55, param56, param57, param58, param59, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {featureMap1, param60, param61, layout}, {out9});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in9},
    {scoresOut1, classesOut1, out9});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nchw_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nchw_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type14(Type::TENSOR_INT32, {0});
  OperandType type16(Type::TENSOR_INT32, {1});
  OperandType type22(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type29(Type::FLOAT16, {});
  OperandType type3(Type::INT32, {});
  OperandType type74(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type75(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type76(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type77(Type::TENSOR_FLOAT16, {0});
  OperandType type82(Type::TENSOR_FLOAT16, {0, 1, 2, 2});
  OperandType type83(Type::TENSOR_FLOAT16, {0, 1, 3, 3});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type76);
  auto roi1 = model->addOperand(&type74);
  auto param47 = model->addOperand(&type16);
  auto param48 = model->addOperand(&type29);
  auto param49 = model->addOperand(&type3);
  auto param50 = model->addOperand(&type3);
  auto param51 = model->addOperand(&type29);
  auto param52 = model->addOperand(&type29);
  auto param53 = model->addOperand(&type29);
  auto scoresOut1 = model->addOperand(&type77);
  auto roiOut1 = model->addOperand(&type75);
  auto classesOut1 = model->addOperand(&type14);
  auto batchSplitOut1 = model->addOperand(&type14);
  auto in9 = model->addOperand(&type22);
  auto param54 = model->addOperand(&type3);
  auto param55 = model->addOperand(&type3);
  auto param56 = model->addOperand(&type29);
  auto param57 = model->addOperand(&type29);
  auto param58 = model->addOperand(&type3);
  auto param59 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type82);
  auto param60 = model->addOperand(&type29);
  auto param61 = model->addOperand(&type29);
  auto out9 = model->addOperand(&type83);
  // Phase 2, operations
  static _Float16 scores1_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores1, scores1_init, sizeof(_Float16) * 2);
  static _Float16 roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(_Float16) * 8);
  static int32_t param47_init[] = {0};
  model->setOperandValue(param47, param47_init, sizeof(int32_t) * 1);
  static _Float16 param48_init[] = {0.30000001192092896f};
  model->setOperandValue(param48, param48_init, sizeof(_Float16) * 1);
  static int32_t param49_init[] = {-1};
  model->setOperandValue(param49, param49_init, sizeof(int32_t) * 1);
  static int32_t param50_init[] = {0};
  model->setOperandValue(param50, param50_init, sizeof(int32_t) * 1);
  static _Float16 param51_init[] = {0.4000000059604645f};
  model->setOperandValue(param51, param51_init, sizeof(_Float16) * 1);
  static _Float16 param52_init[] = {1.0f};
  model->setOperandValue(param52, param52_init, sizeof(_Float16) * 1);
  static _Float16 param53_init[] = {0.30000001192092896f};
  model->setOperandValue(param53, param53_init, sizeof(_Float16) * 1);
  static int32_t param54_init[] = {2};
  model->setOperandValue(param54, param54_init, sizeof(int32_t) * 1);
  static int32_t param55_init[] = {2};
  model->setOperandValue(param55, param55_init, sizeof(int32_t) * 1);
  static _Float16 param56_init[] = {2.0f};
  model->setOperandValue(param56, param56_init, sizeof(_Float16) * 1);
  static _Float16 param57_init[] = {2.0f};
  model->setOperandValue(param57, param57_init, sizeof(_Float16) * 1);
  static int32_t param58_init[] = {4};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static int32_t param59_init[] = {4};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static _Float16 param60_init[] = {1.600000023841858f};
  model->setOperandValue(param60, param60_init, sizeof(_Float16) * 1);
  static _Float16 param61_init[] = {1.600000023841858f};
  model->setOperandValue(param61, param61_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param47, param48, param49, param50, param51, param52, param53}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in9, roiOut1, batchSplitOut1, param54, param55, param56, param57, param58, param59, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {featureMap1, param60, param61, layout}, {out9});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in9},
    {scoresOut1, classesOut1, out9});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nchw_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nhwc_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type12(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type13(Type::TENSOR_FLOAT32, {0});
  OperandType type14(Type::TENSOR_INT32, {0});
  OperandType type15(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type16(Type::TENSOR_INT32, {1});
  OperandType type17(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type11);
  auto roi1 = model->addOperand(&type12);
  auto param47 = model->addOperand(&type16);
  auto param48 = model->addOperand(&type4);
  auto param49 = model->addOperand(&type3);
  auto param50 = model->addOperand(&type3);
  auto param51 = model->addOperand(&type4);
  auto param52 = model->addOperand(&type4);
  auto param53 = model->addOperand(&type4);
  auto scoresOut1 = model->addOperand(&type13);
  auto roiOut1 = model->addOperand(&type15);
  auto classesOut1 = model->addOperand(&type14);
  auto batchSplitOut1 = model->addOperand(&type14);
  auto in9 = model->addOperand(&type2);
  auto param54 = model->addOperand(&type3);
  auto param55 = model->addOperand(&type3);
  auto param56 = model->addOperand(&type4);
  auto param57 = model->addOperand(&type4);
  auto param58 = model->addOperand(&type3);
  auto param59 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type17);
  auto param60 = model->addOperand(&type4);
  auto param61 = model->addOperand(&type4);
  auto out9 = model->addOperand(&type26);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param47_init[] = {0};
  model->setOperandValue(param47, param47_init, sizeof(int32_t) * 1);
  static float param48_init[] = {0.3f};
  model->setOperandValue(param48, param48_init, sizeof(float) * 1);
  static int32_t param49_init[] = {-1};
  model->setOperandValue(param49, param49_init, sizeof(int32_t) * 1);
  static int32_t param50_init[] = {0};
  model->setOperandValue(param50, param50_init, sizeof(int32_t) * 1);
  static float param51_init[] = {0.4f};
  model->setOperandValue(param51, param51_init, sizeof(float) * 1);
  static float param52_init[] = {1.0f};
  model->setOperandValue(param52, param52_init, sizeof(float) * 1);
  static float param53_init[] = {0.3f};
  model->setOperandValue(param53, param53_init, sizeof(float) * 1);
  static int32_t param54_init[] = {2};
  model->setOperandValue(param54, param54_init, sizeof(int32_t) * 1);
  static int32_t param55_init[] = {2};
  model->setOperandValue(param55, param55_init, sizeof(int32_t) * 1);
  static float param56_init[] = {2.0f};
  model->setOperandValue(param56, param56_init, sizeof(float) * 1);
  static float param57_init[] = {2.0f};
  model->setOperandValue(param57, param57_init, sizeof(float) * 1);
  static int32_t param58_init[] = {4};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static int32_t param59_init[] = {4};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float param60_init[] = {1.6f};
  model->setOperandValue(param60, param60_init, sizeof(float) * 1);
  static float param61_init[] = {1.6f};
  model->setOperandValue(param61, param61_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param47, param48, param49, param50, param51, param52, param53}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in9, roiOut1, batchSplitOut1, param54, param55, param56, param57, param58, param59, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {featureMap1, param60, param61, layout}, {out9});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in9},
    {scoresOut1, classesOut1, out9});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nhwc_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nhwc_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type12(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type13(Type::TENSOR_FLOAT32, {0});
  OperandType type14(Type::TENSOR_INT32, {0});
  OperandType type15(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type16(Type::TENSOR_INT32, {1});
  OperandType type17(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type11);
  auto roi1 = model->addOperand(&type12);
  auto param47 = model->addOperand(&type16);
  auto param48 = model->addOperand(&type4);
  auto param49 = model->addOperand(&type3);
  auto param50 = model->addOperand(&type3);
  auto param51 = model->addOperand(&type4);
  auto param52 = model->addOperand(&type4);
  auto param53 = model->addOperand(&type4);
  auto scoresOut1 = model->addOperand(&type13);
  auto roiOut1 = model->addOperand(&type15);
  auto classesOut1 = model->addOperand(&type14);
  auto batchSplitOut1 = model->addOperand(&type14);
  auto in9 = model->addOperand(&type2);
  auto param54 = model->addOperand(&type3);
  auto param55 = model->addOperand(&type3);
  auto param56 = model->addOperand(&type4);
  auto param57 = model->addOperand(&type4);
  auto param58 = model->addOperand(&type3);
  auto param59 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type17);
  auto param60 = model->addOperand(&type4);
  auto param61 = model->addOperand(&type4);
  auto out9 = model->addOperand(&type26);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param47_init[] = {0};
  model->setOperandValue(param47, param47_init, sizeof(int32_t) * 1);
  static float param48_init[] = {0.3f};
  model->setOperandValue(param48, param48_init, sizeof(float) * 1);
  static int32_t param49_init[] = {-1};
  model->setOperandValue(param49, param49_init, sizeof(int32_t) * 1);
  static int32_t param50_init[] = {0};
  model->setOperandValue(param50, param50_init, sizeof(int32_t) * 1);
  static float param51_init[] = {0.4f};
  model->setOperandValue(param51, param51_init, sizeof(float) * 1);
  static float param52_init[] = {1.0f};
  model->setOperandValue(param52, param52_init, sizeof(float) * 1);
  static float param53_init[] = {0.3f};
  model->setOperandValue(param53, param53_init, sizeof(float) * 1);
  static int32_t param54_init[] = {2};
  model->setOperandValue(param54, param54_init, sizeof(int32_t) * 1);
  static int32_t param55_init[] = {2};
  model->setOperandValue(param55, param55_init, sizeof(int32_t) * 1);
  static float param56_init[] = {2.0f};
  model->setOperandValue(param56, param56_init, sizeof(float) * 1);
  static float param57_init[] = {2.0f};
  model->setOperandValue(param57, param57_init, sizeof(float) * 1);
  static int32_t param58_init[] = {4};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static int32_t param59_init[] = {4};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float param60_init[] = {1.6f};
  model->setOperandValue(param60, param60_init, sizeof(float) * 1);
  static float param61_init[] = {1.6f};
  model->setOperandValue(param61, param61_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param47, param48, param49, param50, param51, param52, param53}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in9, roiOut1, batchSplitOut1, param54, param55, param56, param57, param58, param59, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {featureMap1, param60, param61, layout}, {out9});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in9},
    {scoresOut1, classesOut1, out9});
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
  OperandType type14(Type::TENSOR_INT32, {0});
  OperandType type16(Type::TENSOR_INT32, {1});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type65(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 1}, 0.1f, 128);
  OperandType type66(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.1f, 128);
  OperandType type68(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type69(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type70(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type71(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  OperandType type84(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 128);
  // Phase 1, operands
  auto scores1 = model->addOperand(&type70);
  auto roi1 = model->addOperand(&type68);
  auto param47 = model->addOperand(&type16);
  auto param48 = model->addOperand(&type4);
  auto param49 = model->addOperand(&type3);
  auto param50 = model->addOperand(&type3);
  auto param51 = model->addOperand(&type4);
  auto param52 = model->addOperand(&type4);
  auto param53 = model->addOperand(&type4);
  auto scoresOut1 = model->addOperand(&type71);
  auto roiOut1 = model->addOperand(&type69);
  auto classesOut1 = model->addOperand(&type14);
  auto batchSplitOut1 = model->addOperand(&type14);
  auto in9 = model->addOperand(&type66);
  auto param54 = model->addOperand(&type3);
  auto param55 = model->addOperand(&type3);
  auto param56 = model->addOperand(&type4);
  auto param57 = model->addOperand(&type4);
  auto param58 = model->addOperand(&type3);
  auto param59 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type65);
  auto param60 = model->addOperand(&type4);
  auto param61 = model->addOperand(&type4);
  auto out9 = model->addOperand(&type84);
  // Phase 2, operations
  static uint8_t scores1_init[] = {137, 129};
  model->setOperandValue(scores1, scores1_init, sizeof(uint8_t) * 2);
  static uint16_t roi1_init[] = {8, 8, 80, 80, 0, 0, 80, 80};
  model->setOperandValue(roi1, roi1_init, sizeof(uint16_t) * 8);
  static int32_t param47_init[] = {0};
  model->setOperandValue(param47, param47_init, sizeof(int32_t) * 1);
  static float param48_init[] = {0.3f};
  model->setOperandValue(param48, param48_init, sizeof(float) * 1);
  static int32_t param49_init[] = {-1};
  model->setOperandValue(param49, param49_init, sizeof(int32_t) * 1);
  static int32_t param50_init[] = {0};
  model->setOperandValue(param50, param50_init, sizeof(int32_t) * 1);
  static float param51_init[] = {0.4f};
  model->setOperandValue(param51, param51_init, sizeof(float) * 1);
  static float param52_init[] = {1.0f};
  model->setOperandValue(param52, param52_init, sizeof(float) * 1);
  static float param53_init[] = {0.3f};
  model->setOperandValue(param53, param53_init, sizeof(float) * 1);
  static int32_t param54_init[] = {2};
  model->setOperandValue(param54, param54_init, sizeof(int32_t) * 1);
  static int32_t param55_init[] = {2};
  model->setOperandValue(param55, param55_init, sizeof(int32_t) * 1);
  static float param56_init[] = {2.0f};
  model->setOperandValue(param56, param56_init, sizeof(float) * 1);
  static float param57_init[] = {2.0f};
  model->setOperandValue(param57, param57_init, sizeof(float) * 1);
  static int32_t param58_init[] = {4};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static int32_t param59_init[] = {4};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float param60_init[] = {1.6f};
  model->setOperandValue(param60, param60_init, sizeof(float) * 1);
  static float param61_init[] = {1.6f};
  model->setOperandValue(param61, param61_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param47, param48, param49, param50, param51, param52, param53}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in9, roiOut1, batchSplitOut1, param54, param55, param56, param57, param58, param59, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {featureMap1, param60, param61, layout}, {out9});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in9},
    {scoresOut1, classesOut1, out9});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nhwc_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nhwc_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type14(Type::TENSOR_INT32, {0});
  OperandType type16(Type::TENSOR_INT32, {1});
  OperandType type22(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type29(Type::FLOAT16, {});
  OperandType type3(Type::INT32, {});
  OperandType type72(Type::TENSOR_FLOAT16, {0, 2, 2, 1});
  OperandType type74(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type75(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type76(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type85(Type::TENSOR_FLOAT16, {0});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type76);
  auto roi1 = model->addOperand(&type74);
  auto param47 = model->addOperand(&type16);
  auto param48 = model->addOperand(&type29);
  auto param49 = model->addOperand(&type3);
  auto param50 = model->addOperand(&type3);
  auto param51 = model->addOperand(&type29);
  auto param52 = model->addOperand(&type29);
  auto param53 = model->addOperand(&type29);
  auto scoresOut1 = model->addOperand(&type85);
  auto roiOut1 = model->addOperand(&type75);
  auto classesOut1 = model->addOperand(&type14);
  auto batchSplitOut1 = model->addOperand(&type14);
  auto in9 = model->addOperand(&type22);
  auto param54 = model->addOperand(&type3);
  auto param55 = model->addOperand(&type3);
  auto param56 = model->addOperand(&type29);
  auto param57 = model->addOperand(&type29);
  auto param58 = model->addOperand(&type3);
  auto param59 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type72);
  auto param60 = model->addOperand(&type29);
  auto param61 = model->addOperand(&type29);
  auto out9 = model->addOperand(&type28);
  // Phase 2, operations
  static _Float16 scores1_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores1, scores1_init, sizeof(_Float16) * 2);
  static _Float16 roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(_Float16) * 8);
  static int32_t param47_init[] = {0};
  model->setOperandValue(param47, param47_init, sizeof(int32_t) * 1);
  static _Float16 param48_init[] = {0.30000001192092896f};
  model->setOperandValue(param48, param48_init, sizeof(_Float16) * 1);
  static int32_t param49_init[] = {-1};
  model->setOperandValue(param49, param49_init, sizeof(int32_t) * 1);
  static int32_t param50_init[] = {0};
  model->setOperandValue(param50, param50_init, sizeof(int32_t) * 1);
  static _Float16 param51_init[] = {0.4000000059604645f};
  model->setOperandValue(param51, param51_init, sizeof(_Float16) * 1);
  static _Float16 param52_init[] = {1.0f};
  model->setOperandValue(param52, param52_init, sizeof(_Float16) * 1);
  static _Float16 param53_init[] = {0.30000001192092896f};
  model->setOperandValue(param53, param53_init, sizeof(_Float16) * 1);
  static int32_t param54_init[] = {2};
  model->setOperandValue(param54, param54_init, sizeof(int32_t) * 1);
  static int32_t param55_init[] = {2};
  model->setOperandValue(param55, param55_init, sizeof(int32_t) * 1);
  static _Float16 param56_init[] = {2.0f};
  model->setOperandValue(param56, param56_init, sizeof(_Float16) * 1);
  static _Float16 param57_init[] = {2.0f};
  model->setOperandValue(param57, param57_init, sizeof(_Float16) * 1);
  static int32_t param58_init[] = {4};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static int32_t param59_init[] = {4};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static _Float16 param60_init[] = {1.600000023841858f};
  model->setOperandValue(param60, param60_init, sizeof(_Float16) * 1);
  static _Float16 param61_init[] = {1.600000023841858f};
  model->setOperandValue(param61, param61_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param47, param48, param49, param50, param51, param52, param53}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in9, roiOut1, batchSplitOut1, param54, param55, param56, param57, param58, param59, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {featureMap1, param60, param61, layout}, {out9});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in9},
    {scoresOut1, classesOut1, out9});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nhwc_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nchw_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type12(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type13(Type::TENSOR_FLOAT32, {0});
  OperandType type14(Type::TENSOR_INT32, {0});
  OperandType type15(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type16(Type::TENSOR_INT32, {1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type78(Type::TENSOR_FLOAT32, {0, 1, 2, 2});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type11);
  auto roi1 = model->addOperand(&type12);
  auto param47 = model->addOperand(&type16);
  auto param48 = model->addOperand(&type4);
  auto param49 = model->addOperand(&type3);
  auto param50 = model->addOperand(&type3);
  auto param51 = model->addOperand(&type4);
  auto param52 = model->addOperand(&type4);
  auto param53 = model->addOperand(&type4);
  auto scoresOut1 = model->addOperand(&type13);
  auto roiOut1 = model->addOperand(&type15);
  auto classesOut1 = model->addOperand(&type14);
  auto batchSplitOut1 = model->addOperand(&type14);
  auto in9 = model->addOperand(&type2);
  auto param54 = model->addOperand(&type3);
  auto param55 = model->addOperand(&type3);
  auto param56 = model->addOperand(&type4);
  auto param57 = model->addOperand(&type4);
  auto param58 = model->addOperand(&type3);
  auto param59 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type78);
  auto param60 = model->addOperand(&type4);
  auto param61 = model->addOperand(&type4);
  auto out9 = model->addOperand(&type26);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param47_init[] = {0};
  model->setOperandValue(param47, param47_init, sizeof(int32_t) * 1);
  static float param48_init[] = {0.3f};
  model->setOperandValue(param48, param48_init, sizeof(float) * 1);
  static int32_t param49_init[] = {-1};
  model->setOperandValue(param49, param49_init, sizeof(int32_t) * 1);
  static int32_t param50_init[] = {0};
  model->setOperandValue(param50, param50_init, sizeof(int32_t) * 1);
  static float param51_init[] = {0.4f};
  model->setOperandValue(param51, param51_init, sizeof(float) * 1);
  static float param52_init[] = {1.0f};
  model->setOperandValue(param52, param52_init, sizeof(float) * 1);
  static float param53_init[] = {0.3f};
  model->setOperandValue(param53, param53_init, sizeof(float) * 1);
  static int32_t param54_init[] = {2};
  model->setOperandValue(param54, param54_init, sizeof(int32_t) * 1);
  static int32_t param55_init[] = {2};
  model->setOperandValue(param55, param55_init, sizeof(int32_t) * 1);
  static float param56_init[] = {2.0f};
  model->setOperandValue(param56, param56_init, sizeof(float) * 1);
  static float param57_init[] = {2.0f};
  model->setOperandValue(param57, param57_init, sizeof(float) * 1);
  static int32_t param58_init[] = {4};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static int32_t param59_init[] = {4};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float param60_init[] = {1.6f};
  model->setOperandValue(param60, param60_init, sizeof(float) * 1);
  static float param61_init[] = {1.6f};
  model->setOperandValue(param61, param61_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param47, param48, param49, param50, param51, param52, param53}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in9, roiOut1, batchSplitOut1, param54, param55, param56, param57, param58, param59, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {featureMap1, param60, param61, layout}, {out9});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in9},
    {scoresOut1, classesOut1, out9});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nchw_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nchw_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type12(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type13(Type::TENSOR_FLOAT32, {0});
  OperandType type14(Type::TENSOR_INT32, {0});
  OperandType type15(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type16(Type::TENSOR_INT32, {1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type78(Type::TENSOR_FLOAT32, {0, 1, 2, 2});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type11);
  auto roi1 = model->addOperand(&type12);
  auto param47 = model->addOperand(&type16);
  auto param48 = model->addOperand(&type4);
  auto param49 = model->addOperand(&type3);
  auto param50 = model->addOperand(&type3);
  auto param51 = model->addOperand(&type4);
  auto param52 = model->addOperand(&type4);
  auto param53 = model->addOperand(&type4);
  auto scoresOut1 = model->addOperand(&type13);
  auto roiOut1 = model->addOperand(&type15);
  auto classesOut1 = model->addOperand(&type14);
  auto batchSplitOut1 = model->addOperand(&type14);
  auto in9 = model->addOperand(&type2);
  auto param54 = model->addOperand(&type3);
  auto param55 = model->addOperand(&type3);
  auto param56 = model->addOperand(&type4);
  auto param57 = model->addOperand(&type4);
  auto param58 = model->addOperand(&type3);
  auto param59 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type78);
  auto param60 = model->addOperand(&type4);
  auto param61 = model->addOperand(&type4);
  auto out9 = model->addOperand(&type26);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param47_init[] = {0};
  model->setOperandValue(param47, param47_init, sizeof(int32_t) * 1);
  static float param48_init[] = {0.3f};
  model->setOperandValue(param48, param48_init, sizeof(float) * 1);
  static int32_t param49_init[] = {-1};
  model->setOperandValue(param49, param49_init, sizeof(int32_t) * 1);
  static int32_t param50_init[] = {0};
  model->setOperandValue(param50, param50_init, sizeof(int32_t) * 1);
  static float param51_init[] = {0.4f};
  model->setOperandValue(param51, param51_init, sizeof(float) * 1);
  static float param52_init[] = {1.0f};
  model->setOperandValue(param52, param52_init, sizeof(float) * 1);
  static float param53_init[] = {0.3f};
  model->setOperandValue(param53, param53_init, sizeof(float) * 1);
  static int32_t param54_init[] = {2};
  model->setOperandValue(param54, param54_init, sizeof(int32_t) * 1);
  static int32_t param55_init[] = {2};
  model->setOperandValue(param55, param55_init, sizeof(int32_t) * 1);
  static float param56_init[] = {2.0f};
  model->setOperandValue(param56, param56_init, sizeof(float) * 1);
  static float param57_init[] = {2.0f};
  model->setOperandValue(param57, param57_init, sizeof(float) * 1);
  static int32_t param58_init[] = {4};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static int32_t param59_init[] = {4};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float param60_init[] = {1.6f};
  model->setOperandValue(param60, param60_init, sizeof(float) * 1);
  static float param61_init[] = {1.6f};
  model->setOperandValue(param61, param61_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param47, param48, param49, param50, param51, param52, param53}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in9, roiOut1, batchSplitOut1, param54, param55, param56, param57, param58, param59, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {featureMap1, param60, param61, layout}, {out9});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in9},
    {scoresOut1, classesOut1, out9});
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
  OperandType type14(Type::TENSOR_INT32, {0});
  OperandType type16(Type::TENSOR_INT32, {1});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type66(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.1f, 128);
  OperandType type68(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type69(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type70(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type71(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  OperandType type80(Type::TENSOR_QUANT8_ASYMM, {0, 1, 2, 2}, 0.1f, 128);
  OperandType type84(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 128);
  // Phase 1, operands
  auto scores1 = model->addOperand(&type70);
  auto roi1 = model->addOperand(&type68);
  auto param47 = model->addOperand(&type16);
  auto param48 = model->addOperand(&type4);
  auto param49 = model->addOperand(&type3);
  auto param50 = model->addOperand(&type3);
  auto param51 = model->addOperand(&type4);
  auto param52 = model->addOperand(&type4);
  auto param53 = model->addOperand(&type4);
  auto scoresOut1 = model->addOperand(&type71);
  auto roiOut1 = model->addOperand(&type69);
  auto classesOut1 = model->addOperand(&type14);
  auto batchSplitOut1 = model->addOperand(&type14);
  auto in9 = model->addOperand(&type66);
  auto param54 = model->addOperand(&type3);
  auto param55 = model->addOperand(&type3);
  auto param56 = model->addOperand(&type4);
  auto param57 = model->addOperand(&type4);
  auto param58 = model->addOperand(&type3);
  auto param59 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type80);
  auto param60 = model->addOperand(&type4);
  auto param61 = model->addOperand(&type4);
  auto out9 = model->addOperand(&type84);
  // Phase 2, operations
  static uint8_t scores1_init[] = {137, 129};
  model->setOperandValue(scores1, scores1_init, sizeof(uint8_t) * 2);
  static uint16_t roi1_init[] = {8, 8, 80, 80, 0, 0, 80, 80};
  model->setOperandValue(roi1, roi1_init, sizeof(uint16_t) * 8);
  static int32_t param47_init[] = {0};
  model->setOperandValue(param47, param47_init, sizeof(int32_t) * 1);
  static float param48_init[] = {0.3f};
  model->setOperandValue(param48, param48_init, sizeof(float) * 1);
  static int32_t param49_init[] = {-1};
  model->setOperandValue(param49, param49_init, sizeof(int32_t) * 1);
  static int32_t param50_init[] = {0};
  model->setOperandValue(param50, param50_init, sizeof(int32_t) * 1);
  static float param51_init[] = {0.4f};
  model->setOperandValue(param51, param51_init, sizeof(float) * 1);
  static float param52_init[] = {1.0f};
  model->setOperandValue(param52, param52_init, sizeof(float) * 1);
  static float param53_init[] = {0.3f};
  model->setOperandValue(param53, param53_init, sizeof(float) * 1);
  static int32_t param54_init[] = {2};
  model->setOperandValue(param54, param54_init, sizeof(int32_t) * 1);
  static int32_t param55_init[] = {2};
  model->setOperandValue(param55, param55_init, sizeof(int32_t) * 1);
  static float param56_init[] = {2.0f};
  model->setOperandValue(param56, param56_init, sizeof(float) * 1);
  static float param57_init[] = {2.0f};
  model->setOperandValue(param57, param57_init, sizeof(float) * 1);
  static int32_t param58_init[] = {4};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static int32_t param59_init[] = {4};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float param60_init[] = {1.6f};
  model->setOperandValue(param60, param60_init, sizeof(float) * 1);
  static float param61_init[] = {1.6f};
  model->setOperandValue(param61, param61_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param47, param48, param49, param50, param51, param52, param53}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in9, roiOut1, batchSplitOut1, param54, param55, param56, param57, param58, param59, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {featureMap1, param60, param61, layout}, {out9});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in9},
    {scoresOut1, classesOut1, out9});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nchw_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nchw_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type14(Type::TENSOR_INT32, {0});
  OperandType type16(Type::TENSOR_INT32, {1});
  OperandType type22(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type29(Type::FLOAT16, {});
  OperandType type3(Type::INT32, {});
  OperandType type74(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type75(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type76(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type82(Type::TENSOR_FLOAT16, {0, 1, 2, 2});
  OperandType type85(Type::TENSOR_FLOAT16, {0});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type76);
  auto roi1 = model->addOperand(&type74);
  auto param47 = model->addOperand(&type16);
  auto param48 = model->addOperand(&type29);
  auto param49 = model->addOperand(&type3);
  auto param50 = model->addOperand(&type3);
  auto param51 = model->addOperand(&type29);
  auto param52 = model->addOperand(&type29);
  auto param53 = model->addOperand(&type29);
  auto scoresOut1 = model->addOperand(&type85);
  auto roiOut1 = model->addOperand(&type75);
  auto classesOut1 = model->addOperand(&type14);
  auto batchSplitOut1 = model->addOperand(&type14);
  auto in9 = model->addOperand(&type22);
  auto param54 = model->addOperand(&type3);
  auto param55 = model->addOperand(&type3);
  auto param56 = model->addOperand(&type29);
  auto param57 = model->addOperand(&type29);
  auto param58 = model->addOperand(&type3);
  auto param59 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type82);
  auto param60 = model->addOperand(&type29);
  auto param61 = model->addOperand(&type29);
  auto out9 = model->addOperand(&type28);
  // Phase 2, operations
  static _Float16 scores1_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores1, scores1_init, sizeof(_Float16) * 2);
  static _Float16 roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(_Float16) * 8);
  static int32_t param47_init[] = {0};
  model->setOperandValue(param47, param47_init, sizeof(int32_t) * 1);
  static _Float16 param48_init[] = {0.30000001192092896f};
  model->setOperandValue(param48, param48_init, sizeof(_Float16) * 1);
  static int32_t param49_init[] = {-1};
  model->setOperandValue(param49, param49_init, sizeof(int32_t) * 1);
  static int32_t param50_init[] = {0};
  model->setOperandValue(param50, param50_init, sizeof(int32_t) * 1);
  static _Float16 param51_init[] = {0.4000000059604645f};
  model->setOperandValue(param51, param51_init, sizeof(_Float16) * 1);
  static _Float16 param52_init[] = {1.0f};
  model->setOperandValue(param52, param52_init, sizeof(_Float16) * 1);
  static _Float16 param53_init[] = {0.30000001192092896f};
  model->setOperandValue(param53, param53_init, sizeof(_Float16) * 1);
  static int32_t param54_init[] = {2};
  model->setOperandValue(param54, param54_init, sizeof(int32_t) * 1);
  static int32_t param55_init[] = {2};
  model->setOperandValue(param55, param55_init, sizeof(int32_t) * 1);
  static _Float16 param56_init[] = {2.0f};
  model->setOperandValue(param56, param56_init, sizeof(_Float16) * 1);
  static _Float16 param57_init[] = {2.0f};
  model->setOperandValue(param57, param57_init, sizeof(_Float16) * 1);
  static int32_t param58_init[] = {4};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static int32_t param59_init[] = {4};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static _Float16 param60_init[] = {1.600000023841858f};
  model->setOperandValue(param60, param60_init, sizeof(_Float16) * 1);
  static _Float16 param61_init[] = {1.600000023841858f};
  model->setOperandValue(param61, param61_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param47, param48, param49, param50, param51, param52, param53}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in9, roiOut1, batchSplitOut1, param54, param55, param56, param57, param58, param59, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_RESIZE_NEAREST_NEIGHBOR, {featureMap1, param60, param61, layout}, {out9});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in9},
    {scoresOut1, classesOut1, out9});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nchw_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

