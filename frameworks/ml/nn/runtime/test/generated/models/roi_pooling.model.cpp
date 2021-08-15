// clang-format off
// Generated file (from: roi_pooling.mod.py). Do not edit
void CreateModel_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 4, 4, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {5, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {5, 2, 2, 1});
  OperandType type4(Type::TENSOR_INT32, {5});
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type1);
  auto roi = model->addOperand(&type2);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type6);
  auto param4 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param_init[] = {0, 0, 0, 0, 0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 5);
  static int32_t param1_init[] = {2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {2};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static float param3_init[] = {2.0f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static float param4_init[] = {2.0f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in, roi, param, param1, param2, param3, param4, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in, roi},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 4, 4, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {5, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {5, 2, 2, 1});
  OperandType type4(Type::TENSOR_INT32, {5});
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type1);
  auto roi = model->addOperand(&type2);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type6);
  auto param4 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param_init[] = {0, 0, 0, 0, 0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 5);
  static int32_t param1_init[] = {2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {2};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static float param3_init[] = {2.0f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static float param4_init[] = {2.0f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in, roi, param, param1, param2, param3, param4, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in, roi},
    {out});
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
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 4, 4, 1}, 0.25f, 128);
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {5, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_QUANT16_ASYMM, {5, 4}, 0.125f, 0);
  OperandType type4(Type::TENSOR_INT32, {5});
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type12);
  auto roi = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type6);
  auto param4 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t param_init[] = {0, 0, 0, 0, 0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 5);
  static int32_t param1_init[] = {2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {2};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static float param3_init[] = {2.0f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static float param4_init[] = {2.0f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in, roi, param, param1, param2, param3, param4, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in, roi},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type15(Type::TENSOR_FLOAT16, {1, 4, 4, 1});
  OperandType type16(Type::TENSOR_FLOAT16, {5, 2, 2, 1});
  OperandType type17(Type::FLOAT16, {});
  OperandType type18(Type::TENSOR_FLOAT16, {5, 4});
  OperandType type4(Type::TENSOR_INT32, {5});
  OperandType type5(Type::INT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type15);
  auto roi = model->addOperand(&type18);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type17);
  auto param4 = model->addOperand(&type17);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type16);
  // Phase 2, operations
  static int32_t param_init[] = {0, 0, 0, 0, 0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 5);
  static int32_t param1_init[] = {2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {2};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static _Float16 param3_init[] = {2.0f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static _Float16 param4_init[] = {2.0f};
  model->setOperandValue(param4, param4_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in, roi, param, param1, param2, param3, param4, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in, roi},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type19(Type::TENSOR_FLOAT32, {1, 1, 4, 4});
  OperandType type2(Type::TENSOR_FLOAT32, {5, 4});
  OperandType type20(Type::TENSOR_FLOAT32, {5, 1, 2, 2});
  OperandType type4(Type::TENSOR_INT32, {5});
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type19);
  auto roi = model->addOperand(&type2);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type6);
  auto param4 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param_init[] = {0, 0, 0, 0, 0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 5);
  static int32_t param1_init[] = {2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {2};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static float param3_init[] = {2.0f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static float param4_init[] = {2.0f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in, roi, param, param1, param2, param3, param4, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in, roi},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type19(Type::TENSOR_FLOAT32, {1, 1, 4, 4});
  OperandType type2(Type::TENSOR_FLOAT32, {5, 4});
  OperandType type20(Type::TENSOR_FLOAT32, {5, 1, 2, 2});
  OperandType type4(Type::TENSOR_INT32, {5});
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type19);
  auto roi = model->addOperand(&type2);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type6);
  auto param4 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param_init[] = {0, 0, 0, 0, 0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 5);
  static int32_t param1_init[] = {2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {2};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static float param3_init[] = {2.0f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static float param4_init[] = {2.0f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in, roi, param, param1, param2, param3, param4, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in, roi},
    {out});
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
  OperandType type14(Type::TENSOR_QUANT16_ASYMM, {5, 4}, 0.125f, 0);
  OperandType type21(Type::TENSOR_QUANT8_ASYMM, {1, 1, 4, 4}, 0.25f, 128);
  OperandType type22(Type::TENSOR_QUANT8_ASYMM, {5, 1, 2, 2}, 0.25f, 128);
  OperandType type4(Type::TENSOR_INT32, {5});
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type21);
  auto roi = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type6);
  auto param4 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type22);
  // Phase 2, operations
  static int32_t param_init[] = {0, 0, 0, 0, 0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 5);
  static int32_t param1_init[] = {2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {2};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static float param3_init[] = {2.0f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static float param4_init[] = {2.0f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in, roi, param, param1, param2, param3, param4, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in, roi},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_nchw_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type17(Type::FLOAT16, {});
  OperandType type18(Type::TENSOR_FLOAT16, {5, 4});
  OperandType type23(Type::TENSOR_FLOAT16, {1, 1, 4, 4});
  OperandType type24(Type::TENSOR_FLOAT16, {5, 1, 2, 2});
  OperandType type4(Type::TENSOR_INT32, {5});
  OperandType type5(Type::INT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type23);
  auto roi = model->addOperand(&type18);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type17);
  auto param4 = model->addOperand(&type17);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param_init[] = {0, 0, 0, 0, 0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 5);
  static int32_t param1_init[] = {2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {2};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static _Float16 param3_init[] = {2.0f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static _Float16 param4_init[] = {2.0f};
  model->setOperandValue(param4, param4_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in, roi, param, param1, param2, param3, param4, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in, roi},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 4, 4, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {5, 4});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::TENSOR_INT32, {5});
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type1);
  auto roi = model->addOperand(&type2);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type6);
  auto param4 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param_init[] = {0, 0, 0, 0, 0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 5);
  static int32_t param1_init[] = {2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {2};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static float param3_init[] = {2.0f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static float param4_init[] = {2.0f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in, roi, param, param1, param2, param3, param4, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in, roi},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 4, 4, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {5, 4});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::TENSOR_INT32, {5});
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type1);
  auto roi = model->addOperand(&type2);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type6);
  auto param4 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param_init[] = {0, 0, 0, 0, 0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 5);
  static int32_t param1_init[] = {2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {2};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static float param3_init[] = {2.0f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static float param4_init[] = {2.0f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in, roi, param, param1, param2, param3, param4, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in, roi},
    {out});
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
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 4, 4, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_QUANT16_ASYMM, {5, 4}, 0.125f, 0);
  OperandType type26(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 128);
  OperandType type4(Type::TENSOR_INT32, {5});
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type12);
  auto roi = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type6);
  auto param4 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param_init[] = {0, 0, 0, 0, 0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 5);
  static int32_t param1_init[] = {2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {2};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static float param3_init[] = {2.0f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static float param4_init[] = {2.0f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in, roi, param, param1, param2, param3, param4, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in, roi},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type15(Type::TENSOR_FLOAT16, {1, 4, 4, 1});
  OperandType type17(Type::FLOAT16, {});
  OperandType type18(Type::TENSOR_FLOAT16, {5, 4});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::TENSOR_INT32, {5});
  OperandType type5(Type::INT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type15);
  auto roi = model->addOperand(&type18);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type17);
  auto param4 = model->addOperand(&type17);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param_init[] = {0, 0, 0, 0, 0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 5);
  static int32_t param1_init[] = {2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {2};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static _Float16 param3_init[] = {2.0f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static _Float16 param4_init[] = {2.0f};
  model->setOperandValue(param4, param4_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in, roi, param, param1, param2, param3, param4, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in, roi},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type19(Type::TENSOR_FLOAT32, {1, 1, 4, 4});
  OperandType type2(Type::TENSOR_FLOAT32, {5, 4});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::TENSOR_INT32, {5});
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type19);
  auto roi = model->addOperand(&type2);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type6);
  auto param4 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param_init[] = {0, 0, 0, 0, 0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 5);
  static int32_t param1_init[] = {2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {2};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static float param3_init[] = {2.0f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static float param4_init[] = {2.0f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in, roi, param, param1, param2, param3, param4, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in, roi},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type19(Type::TENSOR_FLOAT32, {1, 1, 4, 4});
  OperandType type2(Type::TENSOR_FLOAT32, {5, 4});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::TENSOR_INT32, {5});
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type19);
  auto roi = model->addOperand(&type2);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type6);
  auto param4 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param_init[] = {0, 0, 0, 0, 0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 5);
  static int32_t param1_init[] = {2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {2};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static float param3_init[] = {2.0f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static float param4_init[] = {2.0f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in, roi, param, param1, param2, param3, param4, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in, roi},
    {out});
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
  OperandType type14(Type::TENSOR_QUANT16_ASYMM, {5, 4}, 0.125f, 0);
  OperandType type21(Type::TENSOR_QUANT8_ASYMM, {1, 1, 4, 4}, 0.25f, 128);
  OperandType type26(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 128);
  OperandType type4(Type::TENSOR_INT32, {5});
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type21);
  auto roi = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type6);
  auto param4 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param_init[] = {0, 0, 0, 0, 0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 5);
  static int32_t param1_init[] = {2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {2};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static float param3_init[] = {2.0f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static float param4_init[] = {2.0f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in, roi, param, param1, param2, param3, param4, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in, roi},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type17(Type::FLOAT16, {});
  OperandType type18(Type::TENSOR_FLOAT16, {5, 4});
  OperandType type23(Type::TENSOR_FLOAT16, {1, 1, 4, 4});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::TENSOR_INT32, {5});
  OperandType type5(Type::INT32, {});
  // Phase 1, operands
  auto in = model->addOperand(&type23);
  auto roi = model->addOperand(&type18);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type17);
  auto param4 = model->addOperand(&type17);
  auto layout = model->addOperand(&type0);
  auto out = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param_init[] = {0, 0, 0, 0, 0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 5);
  static int32_t param1_init[] = {2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {2};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static _Float16 param3_init[] = {2.0f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static _Float16 param4_init[] = {2.0f};
  model->setOperandValue(param4, param4_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in, roi, param, param1, param2, param3, param4, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in, roi},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {4});
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {4, 4, 8, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {4, 4});
  OperandType type9(Type::TENSOR_FLOAT32, {4, 2, 3, 2});
  // Phase 1, operands
  auto in1 = model->addOperand(&type7);
  auto roi1 = model->addOperand(&type8);
  auto param5 = model->addOperand(&type10);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto param8 = model->addOperand(&type6);
  auto param9 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type9);
  // Phase 2, operations
  static int32_t param5_init[] = {0, 0, 3, 3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 4);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {3};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static float param8_init[] = {4.0f};
  model->setOperandValue(param8, param8_init, sizeof(float) * 1);
  static float param9_init[] = {4.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in1, roi1, param5, param6, param7, param8, param9, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1, roi1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {4});
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {4, 4, 8, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {4, 4});
  OperandType type9(Type::TENSOR_FLOAT32, {4, 2, 3, 2});
  // Phase 1, operands
  auto in1 = model->addOperand(&type7);
  auto roi1 = model->addOperand(&type8);
  auto param5 = model->addOperand(&type10);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto param8 = model->addOperand(&type6);
  auto param9 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type9);
  // Phase 2, operations
  static int32_t param5_init[] = {0, 0, 3, 3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 4);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {3};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static float param8_init[] = {4.0f};
  model->setOperandValue(param8, param8_init, sizeof(float) * 1);
  static float param9_init[] = {4.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in1, roi1, param5, param6, param7, param8, param9, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1, roi1},
    {out1});
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
  OperandType type10(Type::TENSOR_INT32, {4});
  OperandType type28(Type::TENSOR_QUANT8_ASYMM, {4, 4, 8, 2}, 0.04f, 0);
  OperandType type29(Type::TENSOR_QUANT8_ASYMM, {4, 2, 3, 2}, 0.04f, 0);
  OperandType type30(Type::TENSOR_QUANT16_ASYMM, {4, 4}, 0.125f, 0);
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  // Phase 1, operands
  auto in1 = model->addOperand(&type28);
  auto roi1 = model->addOperand(&type30);
  auto param5 = model->addOperand(&type10);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto param8 = model->addOperand(&type6);
  auto param9 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type29);
  // Phase 2, operations
  static int32_t param5_init[] = {0, 0, 3, 3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 4);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {3};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static float param8_init[] = {4.0f};
  model->setOperandValue(param8, param8_init, sizeof(float) * 1);
  static float param9_init[] = {4.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in1, roi1, param5, param6, param7, param8, param9, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1, roi1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {4});
  OperandType type17(Type::FLOAT16, {});
  OperandType type31(Type::TENSOR_FLOAT16, {4, 4, 8, 2});
  OperandType type32(Type::TENSOR_FLOAT16, {4, 2, 3, 2});
  OperandType type33(Type::TENSOR_FLOAT16, {4, 4});
  OperandType type5(Type::INT32, {});
  // Phase 1, operands
  auto in1 = model->addOperand(&type31);
  auto roi1 = model->addOperand(&type33);
  auto param5 = model->addOperand(&type10);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto param8 = model->addOperand(&type17);
  auto param9 = model->addOperand(&type17);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type32);
  // Phase 2, operations
  static int32_t param5_init[] = {0, 0, 3, 3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 4);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {3};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static _Float16 param8_init[] = {4.0f};
  model->setOperandValue(param8, param8_init, sizeof(_Float16) * 1);
  static _Float16 param9_init[] = {4.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in1, roi1, param5, param6, param7, param8, param9, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1, roi1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {4});
  OperandType type34(Type::TENSOR_FLOAT32, {4, 2, 4, 8});
  OperandType type35(Type::TENSOR_FLOAT32, {4, 2, 2, 3});
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {4, 4});
  // Phase 1, operands
  auto in1 = model->addOperand(&type34);
  auto roi1 = model->addOperand(&type8);
  auto param5 = model->addOperand(&type10);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto param8 = model->addOperand(&type6);
  auto param9 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type35);
  // Phase 2, operations
  static int32_t param5_init[] = {0, 0, 3, 3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 4);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {3};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static float param8_init[] = {4.0f};
  model->setOperandValue(param8, param8_init, sizeof(float) * 1);
  static float param9_init[] = {4.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in1, roi1, param5, param6, param7, param8, param9, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1, roi1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_nchw_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {4});
  OperandType type34(Type::TENSOR_FLOAT32, {4, 2, 4, 8});
  OperandType type35(Type::TENSOR_FLOAT32, {4, 2, 2, 3});
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {4, 4});
  // Phase 1, operands
  auto in1 = model->addOperand(&type34);
  auto roi1 = model->addOperand(&type8);
  auto param5 = model->addOperand(&type10);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto param8 = model->addOperand(&type6);
  auto param9 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type35);
  // Phase 2, operations
  static int32_t param5_init[] = {0, 0, 3, 3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 4);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {3};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static float param8_init[] = {4.0f};
  model->setOperandValue(param8, param8_init, sizeof(float) * 1);
  static float param9_init[] = {4.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in1, roi1, param5, param6, param7, param8, param9, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1, roi1},
    {out1});
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
  OperandType type10(Type::TENSOR_INT32, {4});
  OperandType type30(Type::TENSOR_QUANT16_ASYMM, {4, 4}, 0.125f, 0);
  OperandType type36(Type::TENSOR_QUANT8_ASYMM, {4, 2, 4, 8}, 0.04f, 0);
  OperandType type37(Type::TENSOR_QUANT8_ASYMM, {4, 2, 2, 3}, 0.04f, 0);
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  // Phase 1, operands
  auto in1 = model->addOperand(&type36);
  auto roi1 = model->addOperand(&type30);
  auto param5 = model->addOperand(&type10);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto param8 = model->addOperand(&type6);
  auto param9 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type37);
  // Phase 2, operations
  static int32_t param5_init[] = {0, 0, 3, 3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 4);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {3};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static float param8_init[] = {4.0f};
  model->setOperandValue(param8, param8_init, sizeof(float) * 1);
  static float param9_init[] = {4.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in1, roi1, param5, param6, param7, param8, param9, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1, roi1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_nchw_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {4});
  OperandType type17(Type::FLOAT16, {});
  OperandType type33(Type::TENSOR_FLOAT16, {4, 4});
  OperandType type38(Type::TENSOR_FLOAT16, {4, 2, 4, 8});
  OperandType type39(Type::TENSOR_FLOAT16, {4, 2, 2, 3});
  OperandType type5(Type::INT32, {});
  // Phase 1, operands
  auto in1 = model->addOperand(&type38);
  auto roi1 = model->addOperand(&type33);
  auto param5 = model->addOperand(&type10);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto param8 = model->addOperand(&type17);
  auto param9 = model->addOperand(&type17);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type39);
  // Phase 2, operations
  static int32_t param5_init[] = {0, 0, 3, 3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 4);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {3};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static _Float16 param8_init[] = {4.0f};
  model->setOperandValue(param8, param8_init, sizeof(_Float16) * 1);
  static _Float16 param9_init[] = {4.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in1, roi1, param5, param6, param7, param8, param9, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1, roi1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_nchw_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {4});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {4, 4, 8, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {4, 4});
  // Phase 1, operands
  auto in1 = model->addOperand(&type7);
  auto roi1 = model->addOperand(&type8);
  auto param5 = model->addOperand(&type10);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto param8 = model->addOperand(&type6);
  auto param9 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param5_init[] = {0, 0, 3, 3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 4);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {3};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static float param8_init[] = {4.0f};
  model->setOperandValue(param8, param8_init, sizeof(float) * 1);
  static float param9_init[] = {4.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in1, roi1, param5, param6, param7, param8, param9, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1, roi1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {4});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {4, 4, 8, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {4, 4});
  // Phase 1, operands
  auto in1 = model->addOperand(&type7);
  auto roi1 = model->addOperand(&type8);
  auto param5 = model->addOperand(&type10);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto param8 = model->addOperand(&type6);
  auto param9 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param5_init[] = {0, 0, 3, 3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 4);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {3};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static float param8_init[] = {4.0f};
  model->setOperandValue(param8, param8_init, sizeof(float) * 1);
  static float param9_init[] = {4.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in1, roi1, param5, param6, param7, param8, param9, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1, roi1},
    {out1});
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
  OperandType type10(Type::TENSOR_INT32, {4});
  OperandType type28(Type::TENSOR_QUANT8_ASYMM, {4, 4, 8, 2}, 0.04f, 0);
  OperandType type30(Type::TENSOR_QUANT16_ASYMM, {4, 4}, 0.125f, 0);
  OperandType type40(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.04f, 0);
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  // Phase 1, operands
  auto in1 = model->addOperand(&type28);
  auto roi1 = model->addOperand(&type30);
  auto param5 = model->addOperand(&type10);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto param8 = model->addOperand(&type6);
  auto param9 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type40);
  // Phase 2, operations
  static int32_t param5_init[] = {0, 0, 3, 3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 4);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {3};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static float param8_init[] = {4.0f};
  model->setOperandValue(param8, param8_init, sizeof(float) * 1);
  static float param9_init[] = {4.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in1, roi1, param5, param6, param7, param8, param9, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1, roi1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {4});
  OperandType type17(Type::FLOAT16, {});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type31(Type::TENSOR_FLOAT16, {4, 4, 8, 2});
  OperandType type33(Type::TENSOR_FLOAT16, {4, 4});
  OperandType type5(Type::INT32, {});
  // Phase 1, operands
  auto in1 = model->addOperand(&type31);
  auto roi1 = model->addOperand(&type33);
  auto param5 = model->addOperand(&type10);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto param8 = model->addOperand(&type17);
  auto param9 = model->addOperand(&type17);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param5_init[] = {0, 0, 3, 3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 4);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {3};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static _Float16 param8_init[] = {4.0f};
  model->setOperandValue(param8, param8_init, sizeof(_Float16) * 1);
  static _Float16 param9_init[] = {4.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in1, roi1, param5, param6, param7, param8, param9, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1, roi1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {4});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type34(Type::TENSOR_FLOAT32, {4, 2, 4, 8});
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {4, 4});
  // Phase 1, operands
  auto in1 = model->addOperand(&type34);
  auto roi1 = model->addOperand(&type8);
  auto param5 = model->addOperand(&type10);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto param8 = model->addOperand(&type6);
  auto param9 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param5_init[] = {0, 0, 3, 3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 4);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {3};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static float param8_init[] = {4.0f};
  model->setOperandValue(param8, param8_init, sizeof(float) * 1);
  static float param9_init[] = {4.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in1, roi1, param5, param6, param7, param8, param9, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1, roi1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {4});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type34(Type::TENSOR_FLOAT32, {4, 2, 4, 8});
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {4, 4});
  // Phase 1, operands
  auto in1 = model->addOperand(&type34);
  auto roi1 = model->addOperand(&type8);
  auto param5 = model->addOperand(&type10);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto param8 = model->addOperand(&type6);
  auto param9 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param5_init[] = {0, 0, 3, 3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 4);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {3};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static float param8_init[] = {4.0f};
  model->setOperandValue(param8, param8_init, sizeof(float) * 1);
  static float param9_init[] = {4.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in1, roi1, param5, param6, param7, param8, param9, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1, roi1},
    {out1});
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
  OperandType type10(Type::TENSOR_INT32, {4});
  OperandType type30(Type::TENSOR_QUANT16_ASYMM, {4, 4}, 0.125f, 0);
  OperandType type36(Type::TENSOR_QUANT8_ASYMM, {4, 2, 4, 8}, 0.04f, 0);
  OperandType type40(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.04f, 0);
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  // Phase 1, operands
  auto in1 = model->addOperand(&type36);
  auto roi1 = model->addOperand(&type30);
  auto param5 = model->addOperand(&type10);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto param8 = model->addOperand(&type6);
  auto param9 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type40);
  // Phase 2, operations
  static int32_t param5_init[] = {0, 0, 3, 3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 4);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {3};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static float param8_init[] = {4.0f};
  model->setOperandValue(param8, param8_init, sizeof(float) * 1);
  static float param9_init[] = {4.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in1, roi1, param5, param6, param7, param8, param9, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1, roi1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {4});
  OperandType type17(Type::FLOAT16, {});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type33(Type::TENSOR_FLOAT16, {4, 4});
  OperandType type38(Type::TENSOR_FLOAT16, {4, 2, 4, 8});
  OperandType type5(Type::INT32, {});
  // Phase 1, operands
  auto in1 = model->addOperand(&type38);
  auto roi1 = model->addOperand(&type33);
  auto param5 = model->addOperand(&type10);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto param8 = model->addOperand(&type17);
  auto param9 = model->addOperand(&type17);
  auto layout = model->addOperand(&type0);
  auto out1 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param5_init[] = {0, 0, 3, 3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 4);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {3};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static _Float16 param8_init[] = {4.0f};
  model->setOperandValue(param8, param8_init, sizeof(_Float16) * 1);
  static _Float16 param9_init[] = {4.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in1, roi1, param5, param6, param7, param8, param9, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1, roi1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {4, 4, 4, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {5, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {5, 2, 2, 1});
  OperandType type4(Type::TENSOR_INT32, {5});
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  // Phase 1, operands
  auto in2 = model->addOperand(&type11);
  auto roi2 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type5);
  auto param12 = model->addOperand(&type5);
  auto param13 = model->addOperand(&type6);
  auto param14 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param10_init[] = {2, 2, 2, 2, 2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 5);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {2.0f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static float param14_init[] = {1.0f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in2, roi2, param10, param11, param12, param13, param14, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2, roi2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relaxed_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {4, 4, 4, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {5, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {5, 2, 2, 1});
  OperandType type4(Type::TENSOR_INT32, {5});
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  // Phase 1, operands
  auto in2 = model->addOperand(&type11);
  auto roi2 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type5);
  auto param12 = model->addOperand(&type5);
  auto param13 = model->addOperand(&type6);
  auto param14 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param10_init[] = {2, 2, 2, 2, 2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 5);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {2.0f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static float param14_init[] = {1.0f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in2, roi2, param10, param11, param12, param13, param14, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2, roi2},
    {out2});
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
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {5, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_QUANT16_ASYMM, {5, 4}, 0.125f, 0);
  OperandType type4(Type::TENSOR_INT32, {5});
  OperandType type41(Type::TENSOR_QUANT8_ASYMM, {4, 4, 4, 1}, 0.25f, 128);
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  // Phase 1, operands
  auto in2 = model->addOperand(&type41);
  auto roi2 = model->addOperand(&type14);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type5);
  auto param12 = model->addOperand(&type5);
  auto param13 = model->addOperand(&type6);
  auto param14 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t param10_init[] = {2, 2, 2, 2, 2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 5);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {2.0f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static float param14_init[] = {1.0f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in2, roi2, param10, param11, param12, param13, param14, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2, roi2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_quant8_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_float16_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type16(Type::TENSOR_FLOAT16, {5, 2, 2, 1});
  OperandType type17(Type::FLOAT16, {});
  OperandType type18(Type::TENSOR_FLOAT16, {5, 4});
  OperandType type4(Type::TENSOR_INT32, {5});
  OperandType type42(Type::TENSOR_FLOAT16, {4, 4, 4, 1});
  OperandType type5(Type::INT32, {});
  // Phase 1, operands
  auto in2 = model->addOperand(&type42);
  auto roi2 = model->addOperand(&type18);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type5);
  auto param12 = model->addOperand(&type5);
  auto param13 = model->addOperand(&type17);
  auto param14 = model->addOperand(&type17);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type16);
  // Phase 2, operations
  static int32_t param10_init[] = {2, 2, 2, 2, 2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 5);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static _Float16 param13_init[] = {2.0f};
  model->setOperandValue(param13, param13_init, sizeof(_Float16) * 1);
  static _Float16 param14_init[] = {1.0f};
  model->setOperandValue(param14, param14_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in2, roi2, param10, param11, param12, param13, param14, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2, roi2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {5, 4});
  OperandType type20(Type::TENSOR_FLOAT32, {5, 1, 2, 2});
  OperandType type4(Type::TENSOR_INT32, {5});
  OperandType type43(Type::TENSOR_FLOAT32, {4, 1, 4, 4});
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  // Phase 1, operands
  auto in2 = model->addOperand(&type43);
  auto roi2 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type5);
  auto param12 = model->addOperand(&type5);
  auto param13 = model->addOperand(&type6);
  auto param14 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param10_init[] = {2, 2, 2, 2, 2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 5);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {2.0f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static float param14_init[] = {1.0f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in2, roi2, param10, param11, param12, param13, param14, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2, roi2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_nchw_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relaxed_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {5, 4});
  OperandType type20(Type::TENSOR_FLOAT32, {5, 1, 2, 2});
  OperandType type4(Type::TENSOR_INT32, {5});
  OperandType type43(Type::TENSOR_FLOAT32, {4, 1, 4, 4});
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  // Phase 1, operands
  auto in2 = model->addOperand(&type43);
  auto roi2 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type5);
  auto param12 = model->addOperand(&type5);
  auto param13 = model->addOperand(&type6);
  auto param14 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param10_init[] = {2, 2, 2, 2, 2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 5);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {2.0f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static float param14_init[] = {1.0f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in2, roi2, param10, param11, param12, param13, param14, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2, roi2},
    {out2});
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
  OperandType type14(Type::TENSOR_QUANT16_ASYMM, {5, 4}, 0.125f, 0);
  OperandType type22(Type::TENSOR_QUANT8_ASYMM, {5, 1, 2, 2}, 0.25f, 128);
  OperandType type4(Type::TENSOR_INT32, {5});
  OperandType type44(Type::TENSOR_QUANT8_ASYMM, {4, 1, 4, 4}, 0.25f, 128);
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  // Phase 1, operands
  auto in2 = model->addOperand(&type44);
  auto roi2 = model->addOperand(&type14);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type5);
  auto param12 = model->addOperand(&type5);
  auto param13 = model->addOperand(&type6);
  auto param14 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type22);
  // Phase 2, operations
  static int32_t param10_init[] = {2, 2, 2, 2, 2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 5);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {2.0f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static float param14_init[] = {1.0f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in2, roi2, param10, param11, param12, param13, param14, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2, roi2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_nchw_quant8_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_float16_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type17(Type::FLOAT16, {});
  OperandType type18(Type::TENSOR_FLOAT16, {5, 4});
  OperandType type24(Type::TENSOR_FLOAT16, {5, 1, 2, 2});
  OperandType type4(Type::TENSOR_INT32, {5});
  OperandType type45(Type::TENSOR_FLOAT16, {4, 1, 4, 4});
  OperandType type5(Type::INT32, {});
  // Phase 1, operands
  auto in2 = model->addOperand(&type45);
  auto roi2 = model->addOperand(&type18);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type5);
  auto param12 = model->addOperand(&type5);
  auto param13 = model->addOperand(&type17);
  auto param14 = model->addOperand(&type17);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param10_init[] = {2, 2, 2, 2, 2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 5);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static _Float16 param13_init[] = {2.0f};
  model->setOperandValue(param13, param13_init, sizeof(_Float16) * 1);
  static _Float16 param14_init[] = {1.0f};
  model->setOperandValue(param14, param14_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in2, roi2, param10, param11, param12, param13, param14, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2, roi2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_nchw_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {4, 4, 4, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {5, 4});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::TENSOR_INT32, {5});
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  // Phase 1, operands
  auto in2 = model->addOperand(&type11);
  auto roi2 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type5);
  auto param12 = model->addOperand(&type5);
  auto param13 = model->addOperand(&type6);
  auto param14 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param10_init[] = {2, 2, 2, 2, 2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 5);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {2.0f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static float param14_init[] = {1.0f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in2, roi2, param10, param11, param12, param13, param14, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2, roi2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relaxed_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {4, 4, 4, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {5, 4});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::TENSOR_INT32, {5});
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  // Phase 1, operands
  auto in2 = model->addOperand(&type11);
  auto roi2 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type5);
  auto param12 = model->addOperand(&type5);
  auto param13 = model->addOperand(&type6);
  auto param14 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param10_init[] = {2, 2, 2, 2, 2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 5);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {2.0f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static float param14_init[] = {1.0f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in2, roi2, param10, param11, param12, param13, param14, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2, roi2},
    {out2});
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
  OperandType type14(Type::TENSOR_QUANT16_ASYMM, {5, 4}, 0.125f, 0);
  OperandType type26(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 128);
  OperandType type4(Type::TENSOR_INT32, {5});
  OperandType type41(Type::TENSOR_QUANT8_ASYMM, {4, 4, 4, 1}, 0.25f, 128);
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  // Phase 1, operands
  auto in2 = model->addOperand(&type41);
  auto roi2 = model->addOperand(&type14);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type5);
  auto param12 = model->addOperand(&type5);
  auto param13 = model->addOperand(&type6);
  auto param14 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param10_init[] = {2, 2, 2, 2, 2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 5);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {2.0f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static float param14_init[] = {1.0f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in2, roi2, param10, param11, param12, param13, param14, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2, roi2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_quant8_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_float16_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type17(Type::FLOAT16, {});
  OperandType type18(Type::TENSOR_FLOAT16, {5, 4});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::TENSOR_INT32, {5});
  OperandType type42(Type::TENSOR_FLOAT16, {4, 4, 4, 1});
  OperandType type5(Type::INT32, {});
  // Phase 1, operands
  auto in2 = model->addOperand(&type42);
  auto roi2 = model->addOperand(&type18);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type5);
  auto param12 = model->addOperand(&type5);
  auto param13 = model->addOperand(&type17);
  auto param14 = model->addOperand(&type17);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param10_init[] = {2, 2, 2, 2, 2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 5);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static _Float16 param13_init[] = {2.0f};
  model->setOperandValue(param13, param13_init, sizeof(_Float16) * 1);
  static _Float16 param14_init[] = {1.0f};
  model->setOperandValue(param14, param14_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in2, roi2, param10, param11, param12, param13, param14, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2, roi2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {5, 4});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::TENSOR_INT32, {5});
  OperandType type43(Type::TENSOR_FLOAT32, {4, 1, 4, 4});
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  // Phase 1, operands
  auto in2 = model->addOperand(&type43);
  auto roi2 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type5);
  auto param12 = model->addOperand(&type5);
  auto param13 = model->addOperand(&type6);
  auto param14 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param10_init[] = {2, 2, 2, 2, 2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 5);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {2.0f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static float param14_init[] = {1.0f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in2, roi2, param10, param11, param12, param13, param14, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2, roi2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relaxed_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {5, 4});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::TENSOR_INT32, {5});
  OperandType type43(Type::TENSOR_FLOAT32, {4, 1, 4, 4});
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  // Phase 1, operands
  auto in2 = model->addOperand(&type43);
  auto roi2 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type5);
  auto param12 = model->addOperand(&type5);
  auto param13 = model->addOperand(&type6);
  auto param14 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param10_init[] = {2, 2, 2, 2, 2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 5);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {2.0f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static float param14_init[] = {1.0f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in2, roi2, param10, param11, param12, param13, param14, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2, roi2},
    {out2});
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
  OperandType type14(Type::TENSOR_QUANT16_ASYMM, {5, 4}, 0.125f, 0);
  OperandType type26(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 128);
  OperandType type4(Type::TENSOR_INT32, {5});
  OperandType type44(Type::TENSOR_QUANT8_ASYMM, {4, 1, 4, 4}, 0.25f, 128);
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::FLOAT32, {});
  // Phase 1, operands
  auto in2 = model->addOperand(&type44);
  auto roi2 = model->addOperand(&type14);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type5);
  auto param12 = model->addOperand(&type5);
  auto param13 = model->addOperand(&type6);
  auto param14 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param10_init[] = {2, 2, 2, 2, 2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 5);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {2.0f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static float param14_init[] = {1.0f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in2, roi2, param10, param11, param12, param13, param14, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2, roi2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_quant8_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_float16_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type17(Type::FLOAT16, {});
  OperandType type18(Type::TENSOR_FLOAT16, {5, 4});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::TENSOR_INT32, {5});
  OperandType type45(Type::TENSOR_FLOAT16, {4, 1, 4, 4});
  OperandType type5(Type::INT32, {});
  // Phase 1, operands
  auto in2 = model->addOperand(&type45);
  auto roi2 = model->addOperand(&type18);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type5);
  auto param12 = model->addOperand(&type5);
  auto param13 = model->addOperand(&type17);
  auto param14 = model->addOperand(&type17);
  auto layout = model->addOperand(&type0);
  auto out2 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param10_init[] = {2, 2, 2, 2, 2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 5);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static _Float16 param13_init[] = {2.0f};
  model->setOperandValue(param13, param13_init, sizeof(_Float16) * 1);
  static _Float16 param14_init[] = {1.0f};
  model->setOperandValue(param14, param14_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_ROI_POOLING, {in2, roi2, param10, param11, param12, param13, param14, layout}, {out2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in2, roi2},
    {out2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

