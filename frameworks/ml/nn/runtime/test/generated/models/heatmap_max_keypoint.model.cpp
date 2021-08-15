// clang-format off
// Generated file (from: heatmap_max_keypoint.mod.py). Do not edit
void CreateModel_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {6, 4, 4, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {6, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {6, 1});
  OperandType type4(Type::TENSOR_FLOAT32, {6, 1, 2});
  // Phase 1, operands
  auto heatmap = model->addOperand(&type1);
  auto boxes = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto score = model->addOperand(&type3);
  auto keypoint = model->addOperand(&type4);
  // Phase 2, operations
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_HEATMAP_MAX_KEYPOINT, {heatmap, boxes, layout}, {score, keypoint});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {heatmap, boxes},
    {score, keypoint});
  assert(model->isValid());
}

inline bool is_ignored_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {6, 4, 4, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {6, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {6, 1});
  OperandType type4(Type::TENSOR_FLOAT32, {6, 1, 2});
  // Phase 1, operands
  auto heatmap = model->addOperand(&type1);
  auto boxes = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto score = model->addOperand(&type3);
  auto keypoint = model->addOperand(&type4);
  // Phase 2, operations
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_HEATMAP_MAX_KEYPOINT, {heatmap, boxes, layout}, {score, keypoint});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {heatmap, boxes},
    {score, keypoint});
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
  OperandType type12(Type::TENSOR_FLOAT16, {6, 4});
  OperandType type13(Type::TENSOR_FLOAT16, {6, 4, 4, 1});
  OperandType type14(Type::TENSOR_FLOAT16, {6, 1, 2});
  OperandType type15(Type::TENSOR_FLOAT16, {6, 1});
  // Phase 1, operands
  auto heatmap = model->addOperand(&type13);
  auto boxes = model->addOperand(&type12);
  auto layout = model->addOperand(&type0);
  auto score = model->addOperand(&type15);
  auto keypoint = model->addOperand(&type14);
  // Phase 2, operations
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_HEATMAP_MAX_KEYPOINT, {heatmap, boxes, layout}, {score, keypoint});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {heatmap, boxes},
    {score, keypoint});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type16(Type::TENSOR_FLOAT32, {6, 1, 4, 4});
  OperandType type2(Type::TENSOR_FLOAT32, {6, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {6, 1});
  OperandType type4(Type::TENSOR_FLOAT32, {6, 1, 2});
  // Phase 1, operands
  auto heatmap = model->addOperand(&type16);
  auto boxes = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto score = model->addOperand(&type3);
  auto keypoint = model->addOperand(&type4);
  // Phase 2, operations
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_HEATMAP_MAX_KEYPOINT, {heatmap, boxes, layout}, {score, keypoint});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {heatmap, boxes},
    {score, keypoint});
  assert(model->isValid());
}

inline bool is_ignored_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type16(Type::TENSOR_FLOAT32, {6, 1, 4, 4});
  OperandType type2(Type::TENSOR_FLOAT32, {6, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {6, 1});
  OperandType type4(Type::TENSOR_FLOAT32, {6, 1, 2});
  // Phase 1, operands
  auto heatmap = model->addOperand(&type16);
  auto boxes = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto score = model->addOperand(&type3);
  auto keypoint = model->addOperand(&type4);
  // Phase 2, operations
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_HEATMAP_MAX_KEYPOINT, {heatmap, boxes, layout}, {score, keypoint});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {heatmap, boxes},
    {score, keypoint});
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
  OperandType type12(Type::TENSOR_FLOAT16, {6, 4});
  OperandType type14(Type::TENSOR_FLOAT16, {6, 1, 2});
  OperandType type15(Type::TENSOR_FLOAT16, {6, 1});
  OperandType type17(Type::TENSOR_FLOAT16, {6, 1, 4, 4});
  // Phase 1, operands
  auto heatmap = model->addOperand(&type17);
  auto boxes = model->addOperand(&type12);
  auto layout = model->addOperand(&type0);
  auto score = model->addOperand(&type15);
  auto keypoint = model->addOperand(&type14);
  // Phase 2, operations
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_HEATMAP_MAX_KEYPOINT, {heatmap, boxes, layout}, {score, keypoint});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {heatmap, boxes},
    {score, keypoint});
  assert(model->isValid());
}

inline bool is_ignored_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {6, 4, 4, 1});
  OperandType type18(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {6, 4});
  // Phase 1, operands
  auto heatmap = model->addOperand(&type1);
  auto boxes = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto score = model->addOperand(&type18);
  auto keypoint = model->addOperand(&type19);
  // Phase 2, operations
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_HEATMAP_MAX_KEYPOINT, {heatmap, boxes, layout}, {score, keypoint});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {heatmap, boxes},
    {score, keypoint});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {6, 4, 4, 1});
  OperandType type18(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {6, 4});
  // Phase 1, operands
  auto heatmap = model->addOperand(&type1);
  auto boxes = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto score = model->addOperand(&type18);
  auto keypoint = model->addOperand(&type19);
  // Phase 2, operations
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_HEATMAP_MAX_KEYPOINT, {heatmap, boxes, layout}, {score, keypoint});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {heatmap, boxes},
    {score, keypoint});
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
  OperandType type12(Type::TENSOR_FLOAT16, {6, 4});
  OperandType type13(Type::TENSOR_FLOAT16, {6, 4, 4, 1});
  OperandType type20(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0});
  // Phase 1, operands
  auto heatmap = model->addOperand(&type13);
  auto boxes = model->addOperand(&type12);
  auto layout = model->addOperand(&type0);
  auto score = model->addOperand(&type20);
  auto keypoint = model->addOperand(&type21);
  // Phase 2, operations
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_HEATMAP_MAX_KEYPOINT, {heatmap, boxes, layout}, {score, keypoint});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {heatmap, boxes},
    {score, keypoint});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type16(Type::TENSOR_FLOAT32, {6, 1, 4, 4});
  OperandType type18(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {6, 4});
  // Phase 1, operands
  auto heatmap = model->addOperand(&type16);
  auto boxes = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto score = model->addOperand(&type18);
  auto keypoint = model->addOperand(&type19);
  // Phase 2, operations
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_HEATMAP_MAX_KEYPOINT, {heatmap, boxes, layout}, {score, keypoint});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {heatmap, boxes},
    {score, keypoint});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type16(Type::TENSOR_FLOAT32, {6, 1, 4, 4});
  OperandType type18(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {6, 4});
  // Phase 1, operands
  auto heatmap = model->addOperand(&type16);
  auto boxes = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto score = model->addOperand(&type18);
  auto keypoint = model->addOperand(&type19);
  // Phase 2, operations
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_HEATMAP_MAX_KEYPOINT, {heatmap, boxes, layout}, {score, keypoint});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {heatmap, boxes},
    {score, keypoint});
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
  OperandType type12(Type::TENSOR_FLOAT16, {6, 4});
  OperandType type17(Type::TENSOR_FLOAT16, {6, 1, 4, 4});
  OperandType type20(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0});
  // Phase 1, operands
  auto heatmap = model->addOperand(&type17);
  auto boxes = model->addOperand(&type12);
  auto layout = model->addOperand(&type0);
  auto score = model->addOperand(&type20);
  auto keypoint = model->addOperand(&type21);
  // Phase 2, operations
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_HEATMAP_MAX_KEYPOINT, {heatmap, boxes, layout}, {score, keypoint});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {heatmap, boxes},
    {score, keypoint});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type5(Type::TENSOR_FLOAT32, {2, 4, 4, 4});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 4});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 4, 2});
  // Phase 1, operands
  auto heatmap1 = model->addOperand(&type5);
  auto boxes1 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto score1 = model->addOperand(&type6);
  auto keypoint1 = model->addOperand(&type7);
  // Phase 2, operations
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_HEATMAP_MAX_KEYPOINT, {heatmap1, boxes1, layout}, {score1, keypoint1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {heatmap1, boxes1},
    {score1, keypoint1});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type5(Type::TENSOR_FLOAT32, {2, 4, 4, 4});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 4});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 4, 2});
  // Phase 1, operands
  auto heatmap1 = model->addOperand(&type5);
  auto boxes1 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto score1 = model->addOperand(&type6);
  auto keypoint1 = model->addOperand(&type7);
  // Phase 2, operations
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_HEATMAP_MAX_KEYPOINT, {heatmap1, boxes1, layout}, {score1, keypoint1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {heatmap1, boxes1},
    {score1, keypoint1});
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
  OperandType type22(Type::TENSOR_FLOAT16, {2, 4});
  OperandType type23(Type::TENSOR_FLOAT16, {2, 4, 4, 4});
  OperandType type24(Type::TENSOR_FLOAT16, {2, 4, 2});
  // Phase 1, operands
  auto heatmap1 = model->addOperand(&type23);
  auto boxes1 = model->addOperand(&type22);
  auto layout = model->addOperand(&type0);
  auto score1 = model->addOperand(&type22);
  auto keypoint1 = model->addOperand(&type24);
  // Phase 2, operations
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_HEATMAP_MAX_KEYPOINT, {heatmap1, boxes1, layout}, {score1, keypoint1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {heatmap1, boxes1},
    {score1, keypoint1});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type25(Type::TENSOR_QUANT16_ASYMM, {2, 4}, 0.125f, 0);
  OperandType type26(Type::TENSOR_QUANT8_ASYMM, {2, 4, 4, 4}, 0.01f, 128);
  OperandType type27(Type::TENSOR_QUANT16_ASYMM, {2, 4, 2}, 0.125f, 0);
  OperandType type28(Type::TENSOR_QUANT8_ASYMM, {2, 4}, 0.01f, 0);
  // Phase 1, operands
  auto heatmap1 = model->addOperand(&type26);
  auto boxes1 = model->addOperand(&type25);
  auto layout = model->addOperand(&type0);
  auto score1 = model->addOperand(&type28);
  auto keypoint1 = model->addOperand(&type27);
  // Phase 2, operations
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_HEATMAP_MAX_KEYPOINT, {heatmap1, boxes1, layout}, {score1, keypoint1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {heatmap1, boxes1},
    {score1, keypoint1});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type5(Type::TENSOR_FLOAT32, {2, 4, 4, 4});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 4});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 4, 2});
  // Phase 1, operands
  auto heatmap1 = model->addOperand(&type5);
  auto boxes1 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto score1 = model->addOperand(&type6);
  auto keypoint1 = model->addOperand(&type7);
  // Phase 2, operations
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_HEATMAP_MAX_KEYPOINT, {heatmap1, boxes1, layout}, {score1, keypoint1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {heatmap1, boxes1},
    {score1, keypoint1});
  assert(model->isValid());
}

inline bool is_ignored_nchw_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type5(Type::TENSOR_FLOAT32, {2, 4, 4, 4});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 4});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 4, 2});
  // Phase 1, operands
  auto heatmap1 = model->addOperand(&type5);
  auto boxes1 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto score1 = model->addOperand(&type6);
  auto keypoint1 = model->addOperand(&type7);
  // Phase 2, operations
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_HEATMAP_MAX_KEYPOINT, {heatmap1, boxes1, layout}, {score1, keypoint1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {heatmap1, boxes1},
    {score1, keypoint1});
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
  OperandType type22(Type::TENSOR_FLOAT16, {2, 4});
  OperandType type23(Type::TENSOR_FLOAT16, {2, 4, 4, 4});
  OperandType type24(Type::TENSOR_FLOAT16, {2, 4, 2});
  // Phase 1, operands
  auto heatmap1 = model->addOperand(&type23);
  auto boxes1 = model->addOperand(&type22);
  auto layout = model->addOperand(&type0);
  auto score1 = model->addOperand(&type22);
  auto keypoint1 = model->addOperand(&type24);
  // Phase 2, operations
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_HEATMAP_MAX_KEYPOINT, {heatmap1, boxes1, layout}, {score1, keypoint1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {heatmap1, boxes1},
    {score1, keypoint1});
  assert(model->isValid());
}

inline bool is_ignored_nchw_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type25(Type::TENSOR_QUANT16_ASYMM, {2, 4}, 0.125f, 0);
  OperandType type26(Type::TENSOR_QUANT8_ASYMM, {2, 4, 4, 4}, 0.01f, 128);
  OperandType type27(Type::TENSOR_QUANT16_ASYMM, {2, 4, 2}, 0.125f, 0);
  OperandType type28(Type::TENSOR_QUANT8_ASYMM, {2, 4}, 0.01f, 0);
  // Phase 1, operands
  auto heatmap1 = model->addOperand(&type26);
  auto boxes1 = model->addOperand(&type25);
  auto layout = model->addOperand(&type0);
  auto score1 = model->addOperand(&type28);
  auto keypoint1 = model->addOperand(&type27);
  // Phase 2, operations
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_HEATMAP_MAX_KEYPOINT, {heatmap1, boxes1, layout}, {score1, keypoint1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {heatmap1, boxes1},
    {score1, keypoint1});
  assert(model->isValid());
}

inline bool is_ignored_nchw_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type18(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type5(Type::TENSOR_FLOAT32, {2, 4, 4, 4});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 4});
  // Phase 1, operands
  auto heatmap1 = model->addOperand(&type5);
  auto boxes1 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto score1 = model->addOperand(&type18);
  auto keypoint1 = model->addOperand(&type19);
  // Phase 2, operations
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_HEATMAP_MAX_KEYPOINT, {heatmap1, boxes1, layout}, {score1, keypoint1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {heatmap1, boxes1},
    {score1, keypoint1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type18(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type5(Type::TENSOR_FLOAT32, {2, 4, 4, 4});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 4});
  // Phase 1, operands
  auto heatmap1 = model->addOperand(&type5);
  auto boxes1 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto score1 = model->addOperand(&type18);
  auto keypoint1 = model->addOperand(&type19);
  // Phase 2, operations
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_HEATMAP_MAX_KEYPOINT, {heatmap1, boxes1, layout}, {score1, keypoint1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {heatmap1, boxes1},
    {score1, keypoint1});
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
  OperandType type20(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type22(Type::TENSOR_FLOAT16, {2, 4});
  OperandType type23(Type::TENSOR_FLOAT16, {2, 4, 4, 4});
  // Phase 1, operands
  auto heatmap1 = model->addOperand(&type23);
  auto boxes1 = model->addOperand(&type22);
  auto layout = model->addOperand(&type0);
  auto score1 = model->addOperand(&type20);
  auto keypoint1 = model->addOperand(&type21);
  // Phase 2, operations
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_HEATMAP_MAX_KEYPOINT, {heatmap1, boxes1, layout}, {score1, keypoint1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {heatmap1, boxes1},
    {score1, keypoint1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type25(Type::TENSOR_QUANT16_ASYMM, {2, 4}, 0.125f, 0);
  OperandType type26(Type::TENSOR_QUANT8_ASYMM, {2, 4, 4, 4}, 0.01f, 128);
  OperandType type29(Type::TENSOR_QUANT8_ASYMM, {0, 0}, 0.01f, 0);
  OperandType type30(Type::TENSOR_QUANT16_ASYMM, {0, 0, 0}, 0.125f, 0);
  // Phase 1, operands
  auto heatmap1 = model->addOperand(&type26);
  auto boxes1 = model->addOperand(&type25);
  auto layout = model->addOperand(&type0);
  auto score1 = model->addOperand(&type29);
  auto keypoint1 = model->addOperand(&type30);
  // Phase 2, operations
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_HEATMAP_MAX_KEYPOINT, {heatmap1, boxes1, layout}, {score1, keypoint1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {heatmap1, boxes1},
    {score1, keypoint1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type18(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type5(Type::TENSOR_FLOAT32, {2, 4, 4, 4});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 4});
  // Phase 1, operands
  auto heatmap1 = model->addOperand(&type5);
  auto boxes1 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto score1 = model->addOperand(&type18);
  auto keypoint1 = model->addOperand(&type19);
  // Phase 2, operations
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_HEATMAP_MAX_KEYPOINT, {heatmap1, boxes1, layout}, {score1, keypoint1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {heatmap1, boxes1},
    {score1, keypoint1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type18(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type5(Type::TENSOR_FLOAT32, {2, 4, 4, 4});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 4});
  // Phase 1, operands
  auto heatmap1 = model->addOperand(&type5);
  auto boxes1 = model->addOperand(&type6);
  auto layout = model->addOperand(&type0);
  auto score1 = model->addOperand(&type18);
  auto keypoint1 = model->addOperand(&type19);
  // Phase 2, operations
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_HEATMAP_MAX_KEYPOINT, {heatmap1, boxes1, layout}, {score1, keypoint1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {heatmap1, boxes1},
    {score1, keypoint1});
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
  OperandType type20(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type22(Type::TENSOR_FLOAT16, {2, 4});
  OperandType type23(Type::TENSOR_FLOAT16, {2, 4, 4, 4});
  // Phase 1, operands
  auto heatmap1 = model->addOperand(&type23);
  auto boxes1 = model->addOperand(&type22);
  auto layout = model->addOperand(&type0);
  auto score1 = model->addOperand(&type20);
  auto keypoint1 = model->addOperand(&type21);
  // Phase 2, operations
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_HEATMAP_MAX_KEYPOINT, {heatmap1, boxes1, layout}, {score1, keypoint1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {heatmap1, boxes1},
    {score1, keypoint1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type25(Type::TENSOR_QUANT16_ASYMM, {2, 4}, 0.125f, 0);
  OperandType type26(Type::TENSOR_QUANT8_ASYMM, {2, 4, 4, 4}, 0.01f, 128);
  OperandType type29(Type::TENSOR_QUANT8_ASYMM, {0, 0}, 0.01f, 0);
  OperandType type30(Type::TENSOR_QUANT16_ASYMM, {0, 0, 0}, 0.125f, 0);
  // Phase 1, operands
  auto heatmap1 = model->addOperand(&type26);
  auto boxes1 = model->addOperand(&type25);
  auto layout = model->addOperand(&type0);
  auto score1 = model->addOperand(&type29);
  auto keypoint1 = model->addOperand(&type30);
  // Phase 2, operations
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_HEATMAP_MAX_KEYPOINT, {heatmap1, boxes1, layout}, {score1, keypoint1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {heatmap1, boxes1},
    {score1, keypoint1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type31(Type::TENSOR_QUANT16_ASYMM, {5, 4}, 0.125f, 0);
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {5, 4, 4, 1}, 0.5f, 128);
  OperandType type33(Type::TENSOR_QUANT16_ASYMM, {5, 1, 2}, 0.125f, 0);
  OperandType type34(Type::TENSOR_QUANT8_ASYMM, {5, 1}, 0.1f, 10);
  // Phase 1, operands
  auto heatmap2 = model->addOperand(&type32);
  auto boxes2 = model->addOperand(&type31);
  auto layout = model->addOperand(&type0);
  auto score2 = model->addOperand(&type34);
  auto keypoint2 = model->addOperand(&type33);
  // Phase 2, operations
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_HEATMAP_MAX_KEYPOINT, {heatmap2, boxes2, layout}, {score2, keypoint2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {heatmap2, boxes2},
    {score2, keypoint2});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type31(Type::TENSOR_QUANT16_ASYMM, {5, 4}, 0.125f, 0);
  OperandType type33(Type::TENSOR_QUANT16_ASYMM, {5, 1, 2}, 0.125f, 0);
  OperandType type34(Type::TENSOR_QUANT8_ASYMM, {5, 1}, 0.1f, 10);
  OperandType type36(Type::TENSOR_QUANT8_ASYMM, {5, 1, 4, 4}, 0.5f, 128);
  // Phase 1, operands
  auto heatmap2 = model->addOperand(&type36);
  auto boxes2 = model->addOperand(&type31);
  auto layout = model->addOperand(&type0);
  auto score2 = model->addOperand(&type34);
  auto keypoint2 = model->addOperand(&type33);
  // Phase 2, operations
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_HEATMAP_MAX_KEYPOINT, {heatmap2, boxes2, layout}, {score2, keypoint2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {heatmap2, boxes2},
    {score2, keypoint2});
  assert(model->isValid());
}

inline bool is_ignored_nchw_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type30(Type::TENSOR_QUANT16_ASYMM, {0, 0, 0}, 0.125f, 0);
  OperandType type31(Type::TENSOR_QUANT16_ASYMM, {5, 4}, 0.125f, 0);
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {5, 4, 4, 1}, 0.5f, 128);
  OperandType type37(Type::TENSOR_QUANT8_ASYMM, {0, 0}, 0.1f, 10);
  // Phase 1, operands
  auto heatmap2 = model->addOperand(&type32);
  auto boxes2 = model->addOperand(&type31);
  auto layout = model->addOperand(&type0);
  auto score2 = model->addOperand(&type37);
  auto keypoint2 = model->addOperand(&type30);
  // Phase 2, operations
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_HEATMAP_MAX_KEYPOINT, {heatmap2, boxes2, layout}, {score2, keypoint2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {heatmap2, boxes2},
    {score2, keypoint2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type30(Type::TENSOR_QUANT16_ASYMM, {0, 0, 0}, 0.125f, 0);
  OperandType type31(Type::TENSOR_QUANT16_ASYMM, {5, 4}, 0.125f, 0);
  OperandType type36(Type::TENSOR_QUANT8_ASYMM, {5, 1, 4, 4}, 0.5f, 128);
  OperandType type37(Type::TENSOR_QUANT8_ASYMM, {0, 0}, 0.1f, 10);
  // Phase 1, operands
  auto heatmap2 = model->addOperand(&type36);
  auto boxes2 = model->addOperand(&type31);
  auto layout = model->addOperand(&type0);
  auto score2 = model->addOperand(&type37);
  auto keypoint2 = model->addOperand(&type30);
  // Phase 2, operations
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_HEATMAP_MAX_KEYPOINT, {heatmap2, boxes2, layout}, {score2, keypoint2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {heatmap2, boxes2},
    {score2, keypoint2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

