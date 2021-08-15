// clang-format off
// Generated file (from: instance_normalization.mod.py). Do not edit
void CreateModel_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {2, 2, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type1);
  auto param = model->addOperand(&type2);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type1);
  // Phase 2, operations
  static float param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {0.0001f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_INSTANCE_NORMALIZATION, {in, param, param1, param2, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {2, 2, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type1);
  auto param = model->addOperand(&type2);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type1);
  // Phase 2, operations
  static float param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {0.0001f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_INSTANCE_NORMALIZATION, {in, param, param1, param2, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
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
  OperandType type3(Type::TENSOR_FLOAT16, {2, 2, 2, 2});
  OperandType type4(Type::FLOAT16, {});
  // Phase 1, operands
  auto in = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type3);
  // Phase 2, operations
  static _Float16 param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(_Float16) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {9.999999747378752e-05f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_INSTANCE_NORMALIZATION, {in, param, param1, param2, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {2, 2, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type1);
  auto param = model->addOperand(&type2);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type1);
  // Phase 2, operations
  static float param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {0.0001f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_INSTANCE_NORMALIZATION, {in, param, param1, param2, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {2, 2, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type1);
  auto param = model->addOperand(&type2);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type1);
  // Phase 2, operations
  static float param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {0.0001f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_INSTANCE_NORMALIZATION, {in, param, param1, param2, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
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
  OperandType type3(Type::TENSOR_FLOAT16, {2, 2, 2, 2});
  OperandType type4(Type::FLOAT16, {});
  // Phase 1, operands
  auto in = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type3);
  // Phase 2, operations
  static _Float16 param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(_Float16) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {9.999999747378752e-05f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_INSTANCE_NORMALIZATION, {in, param, param1, param2, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {2, 2, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto in = model->addOperand(&type1);
  auto param = model->addOperand(&type2);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type5);
  // Phase 2, operations
  static float param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {0.0001f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_INSTANCE_NORMALIZATION, {in, param, param1, param2, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {2, 2, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto in = model->addOperand(&type1);
  auto param = model->addOperand(&type2);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type5);
  // Phase 2, operations
  static float param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {0.0001f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_INSTANCE_NORMALIZATION, {in, param, param1, param2, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
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
  OperandType type3(Type::TENSOR_FLOAT16, {2, 2, 2, 2});
  OperandType type4(Type::FLOAT16, {});
  OperandType type6(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto in = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type6);
  // Phase 2, operations
  static _Float16 param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(_Float16) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {9.999999747378752e-05f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_INSTANCE_NORMALIZATION, {in, param, param1, param2, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {2, 2, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto in = model->addOperand(&type1);
  auto param = model->addOperand(&type2);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type5);
  // Phase 2, operations
  static float param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {0.0001f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_INSTANCE_NORMALIZATION, {in, param, param1, param2, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {2, 2, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto in = model->addOperand(&type1);
  auto param = model->addOperand(&type2);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type5);
  // Phase 2, operations
  static float param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {0.0001f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_INSTANCE_NORMALIZATION, {in, param, param1, param2, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
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
  OperandType type3(Type::TENSOR_FLOAT16, {2, 2, 2, 2});
  OperandType type4(Type::FLOAT16, {});
  OperandType type6(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto in = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type6);
  // Phase 2, operations
  static _Float16 param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(_Float16) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {9.999999747378752e-05f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_INSTANCE_NORMALIZATION, {in, param, param1, param2, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {2, 2, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto in1 = model->addOperand(&type1);
  auto param3 = model->addOperand(&type2);
  auto param4 = model->addOperand(&type2);
  auto param5 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type1);
  // Phase 2, operations
  static float param3_init[] = {2.0f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static float param4_init[] = {10.0f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {0.0001f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_INSTANCE_NORMALIZATION, {in1, param3, param4, param5, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {2, 2, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto in1 = model->addOperand(&type1);
  auto param3 = model->addOperand(&type2);
  auto param4 = model->addOperand(&type2);
  auto param5 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type1);
  // Phase 2, operations
  static float param3_init[] = {2.0f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static float param4_init[] = {10.0f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {0.0001f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_INSTANCE_NORMALIZATION, {in1, param3, param4, param5, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
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
  OperandType type3(Type::TENSOR_FLOAT16, {2, 2, 2, 2});
  OperandType type4(Type::FLOAT16, {});
  // Phase 1, operands
  auto in1 = model->addOperand(&type3);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type3);
  // Phase 2, operations
  static _Float16 param3_init[] = {2.0f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static _Float16 param4_init[] = {10.0f};
  model->setOperandValue(param4, param4_init, sizeof(_Float16) * 1);
  static _Float16 param5_init[] = {9.999999747378752e-05f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_INSTANCE_NORMALIZATION, {in1, param3, param4, param5, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {2, 2, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto in1 = model->addOperand(&type1);
  auto param3 = model->addOperand(&type2);
  auto param4 = model->addOperand(&type2);
  auto param5 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type1);
  // Phase 2, operations
  static float param3_init[] = {2.0f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static float param4_init[] = {10.0f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {0.0001f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_INSTANCE_NORMALIZATION, {in1, param3, param4, param5, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_nchw_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {2, 2, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto in1 = model->addOperand(&type1);
  auto param3 = model->addOperand(&type2);
  auto param4 = model->addOperand(&type2);
  auto param5 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type1);
  // Phase 2, operations
  static float param3_init[] = {2.0f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static float param4_init[] = {10.0f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {0.0001f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_INSTANCE_NORMALIZATION, {in1, param3, param4, param5, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
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
  OperandType type3(Type::TENSOR_FLOAT16, {2, 2, 2, 2});
  OperandType type4(Type::FLOAT16, {});
  // Phase 1, operands
  auto in1 = model->addOperand(&type3);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type3);
  // Phase 2, operations
  static _Float16 param3_init[] = {2.0f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static _Float16 param4_init[] = {10.0f};
  model->setOperandValue(param4, param4_init, sizeof(_Float16) * 1);
  static _Float16 param5_init[] = {9.999999747378752e-05f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_INSTANCE_NORMALIZATION, {in1, param3, param4, param5, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_nchw_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {2, 2, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto in1 = model->addOperand(&type1);
  auto param3 = model->addOperand(&type2);
  auto param4 = model->addOperand(&type2);
  auto param5 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type5);
  // Phase 2, operations
  static float param3_init[] = {2.0f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static float param4_init[] = {10.0f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {0.0001f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_INSTANCE_NORMALIZATION, {in1, param3, param4, param5, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {2, 2, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto in1 = model->addOperand(&type1);
  auto param3 = model->addOperand(&type2);
  auto param4 = model->addOperand(&type2);
  auto param5 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type5);
  // Phase 2, operations
  static float param3_init[] = {2.0f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static float param4_init[] = {10.0f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {0.0001f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_INSTANCE_NORMALIZATION, {in1, param3, param4, param5, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
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
  OperandType type3(Type::TENSOR_FLOAT16, {2, 2, 2, 2});
  OperandType type4(Type::FLOAT16, {});
  OperandType type6(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto in1 = model->addOperand(&type3);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type6);
  // Phase 2, operations
  static _Float16 param3_init[] = {2.0f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static _Float16 param4_init[] = {10.0f};
  model->setOperandValue(param4, param4_init, sizeof(_Float16) * 1);
  static _Float16 param5_init[] = {9.999999747378752e-05f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_INSTANCE_NORMALIZATION, {in1, param3, param4, param5, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {2, 2, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto in1 = model->addOperand(&type1);
  auto param3 = model->addOperand(&type2);
  auto param4 = model->addOperand(&type2);
  auto param5 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type5);
  // Phase 2, operations
  static float param3_init[] = {2.0f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static float param4_init[] = {10.0f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {0.0001f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_INSTANCE_NORMALIZATION, {in1, param3, param4, param5, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {2, 2, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto in1 = model->addOperand(&type1);
  auto param3 = model->addOperand(&type2);
  auto param4 = model->addOperand(&type2);
  auto param5 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type5);
  // Phase 2, operations
  static float param3_init[] = {2.0f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static float param4_init[] = {10.0f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {0.0001f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_INSTANCE_NORMALIZATION, {in1, param3, param4, param5, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
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
  OperandType type3(Type::TENSOR_FLOAT16, {2, 2, 2, 2});
  OperandType type4(Type::FLOAT16, {});
  OperandType type6(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto in1 = model->addOperand(&type3);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type6);
  // Phase 2, operations
  static _Float16 param3_init[] = {2.0f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static _Float16 param4_init[] = {10.0f};
  model->setOperandValue(param4, param4_init, sizeof(_Float16) * 1);
  static _Float16 param5_init[] = {9.999999747378752e-05f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_INSTANCE_NORMALIZATION, {in1, param3, param4, param5, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

