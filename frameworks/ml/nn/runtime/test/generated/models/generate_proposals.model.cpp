// clang-format off
// Generated file (from: generate_proposals.mod.py). Do not edit
void CreateModel_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 8});
  OperandType type3(Type::TENSOR_FLOAT32, {2, 4});
  OperandType type4(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type5(Type::TENSOR_FLOAT32, {4});
  OperandType type6(Type::TENSOR_FLOAT32, {4, 4});
  OperandType type7(Type::TENSOR_INT32, {4});
  OperandType type8(Type::FLOAT32, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type1);
  auto bboxDeltas = model->addOperand(&type2);
  auto anchors = model->addOperand(&type3);
  auto imageInfo = model->addOperand(&type4);
  auto param = model->addOperand(&type8);
  auto param1 = model->addOperand(&type8);
  auto param2 = model->addOperand(&type9);
  auto param3 = model->addOperand(&type9);
  auto param4 = model->addOperand(&type8);
  auto param5 = model->addOperand(&type8);
  auto layout = model->addOperand(&type0);
  auto scoresOut = model->addOperand(&type5);
  auto roiOut = model->addOperand(&type6);
  auto batchSplit = model->addOperand(&type7);
  // Phase 2, operations
  static float param_init[] = {4.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  static float param1_init[] = {4.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static int32_t param2_init[] = {-1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {-1};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static float param4_init[] = {0.3f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {1.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GENERATE_PROPOSALS, {scores, bboxDeltas, anchors, imageInfo, param, param1, param2, param3, param4, param5, layout}, {scoresOut, roiOut, batchSplit});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores, bboxDeltas, anchors, imageInfo},
    {scoresOut, roiOut, batchSplit});
  assert(model->isValid());
}

inline bool is_ignored_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 8});
  OperandType type3(Type::TENSOR_FLOAT32, {2, 4});
  OperandType type4(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type5(Type::TENSOR_FLOAT32, {4});
  OperandType type6(Type::TENSOR_FLOAT32, {4, 4});
  OperandType type7(Type::TENSOR_INT32, {4});
  OperandType type8(Type::FLOAT32, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type1);
  auto bboxDeltas = model->addOperand(&type2);
  auto anchors = model->addOperand(&type3);
  auto imageInfo = model->addOperand(&type4);
  auto param = model->addOperand(&type8);
  auto param1 = model->addOperand(&type8);
  auto param2 = model->addOperand(&type9);
  auto param3 = model->addOperand(&type9);
  auto param4 = model->addOperand(&type8);
  auto param5 = model->addOperand(&type8);
  auto layout = model->addOperand(&type0);
  auto scoresOut = model->addOperand(&type5);
  auto roiOut = model->addOperand(&type6);
  auto batchSplit = model->addOperand(&type7);
  // Phase 2, operations
  static float param_init[] = {4.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  static float param1_init[] = {4.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static int32_t param2_init[] = {-1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {-1};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static float param4_init[] = {0.3f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {1.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GENERATE_PROPOSALS, {scores, bboxDeltas, anchors, imageInfo, param, param1, param2, param3, param4, param5, layout}, {scoresOut, roiOut, batchSplit});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores, bboxDeltas, anchors, imageInfo},
    {scoresOut, roiOut, batchSplit});
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
  OperandType type16(Type::TENSOR_QUANT16_SYMM, {2, 4}, 0.125f, 0);
  OperandType type17(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 8}, 0.05f, 128);
  OperandType type18(Type::TENSOR_QUANT16_ASYMM, {1, 2}, 0.125f, 0);
  OperandType type19(Type::TENSOR_QUANT16_ASYMM, {4, 4}, 0.125f, 0);
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.01f, 100);
  OperandType type21(Type::TENSOR_QUANT8_ASYMM, {4}, 0.01f, 100);
  OperandType type7(Type::TENSOR_INT32, {4});
  OperandType type8(Type::FLOAT32, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type20);
  auto bboxDeltas = model->addOperand(&type17);
  auto anchors = model->addOperand(&type16);
  auto imageInfo = model->addOperand(&type18);
  auto param = model->addOperand(&type8);
  auto param1 = model->addOperand(&type8);
  auto param2 = model->addOperand(&type9);
  auto param3 = model->addOperand(&type9);
  auto param4 = model->addOperand(&type8);
  auto param5 = model->addOperand(&type8);
  auto layout = model->addOperand(&type0);
  auto scoresOut = model->addOperand(&type21);
  auto roiOut = model->addOperand(&type19);
  auto batchSplit = model->addOperand(&type7);
  // Phase 2, operations
  static float param_init[] = {4.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  static float param1_init[] = {4.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static int32_t param2_init[] = {-1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {-1};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static float param4_init[] = {0.3f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {1.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GENERATE_PROPOSALS, {scores, bboxDeltas, anchors, imageInfo, param, param1, param2, param3, param4, param5, layout}, {scoresOut, roiOut, batchSplit});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores, bboxDeltas, anchors, imageInfo},
    {scoresOut, roiOut, batchSplit});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type22(Type::TENSOR_FLOAT16, {2, 4});
  OperandType type23(Type::TENSOR_FLOAT16, {1, 2, 2, 8});
  OperandType type24(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type25(Type::FLOAT16, {});
  OperandType type26(Type::TENSOR_FLOAT16, {4, 4});
  OperandType type27(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type28(Type::TENSOR_FLOAT16, {4});
  OperandType type7(Type::TENSOR_INT32, {4});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type27);
  auto bboxDeltas = model->addOperand(&type23);
  auto anchors = model->addOperand(&type22);
  auto imageInfo = model->addOperand(&type24);
  auto param = model->addOperand(&type25);
  auto param1 = model->addOperand(&type25);
  auto param2 = model->addOperand(&type9);
  auto param3 = model->addOperand(&type9);
  auto param4 = model->addOperand(&type25);
  auto param5 = model->addOperand(&type25);
  auto layout = model->addOperand(&type0);
  auto scoresOut = model->addOperand(&type28);
  auto roiOut = model->addOperand(&type26);
  auto batchSplit = model->addOperand(&type7);
  // Phase 2, operations
  static _Float16 param_init[] = {4.0f};
  model->setOperandValue(param, param_init, sizeof(_Float16) * 1);
  static _Float16 param1_init[] = {4.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static int32_t param2_init[] = {-1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {-1};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static _Float16 param4_init[] = {0.30000001192092896f};
  model->setOperandValue(param4, param4_init, sizeof(_Float16) * 1);
  static _Float16 param5_init[] = {1.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GENERATE_PROPOSALS, {scores, bboxDeltas, anchors, imageInfo, param, param1, param2, param3, param4, param5, layout}, {scoresOut, roiOut, batchSplit});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores, bboxDeltas, anchors, imageInfo},
    {scoresOut, roiOut, batchSplit});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type29(Type::TENSOR_FLOAT32, {1, 8, 2, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {2, 4});
  OperandType type4(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type5(Type::TENSOR_FLOAT32, {4});
  OperandType type6(Type::TENSOR_FLOAT32, {4, 4});
  OperandType type7(Type::TENSOR_INT32, {4});
  OperandType type8(Type::FLOAT32, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type1);
  auto bboxDeltas = model->addOperand(&type29);
  auto anchors = model->addOperand(&type3);
  auto imageInfo = model->addOperand(&type4);
  auto param = model->addOperand(&type8);
  auto param1 = model->addOperand(&type8);
  auto param2 = model->addOperand(&type9);
  auto param3 = model->addOperand(&type9);
  auto param4 = model->addOperand(&type8);
  auto param5 = model->addOperand(&type8);
  auto layout = model->addOperand(&type0);
  auto scoresOut = model->addOperand(&type5);
  auto roiOut = model->addOperand(&type6);
  auto batchSplit = model->addOperand(&type7);
  // Phase 2, operations
  static float param_init[] = {4.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  static float param1_init[] = {4.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static int32_t param2_init[] = {-1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {-1};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static float param4_init[] = {0.3f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {1.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GENERATE_PROPOSALS, {scores, bboxDeltas, anchors, imageInfo, param, param1, param2, param3, param4, param5, layout}, {scoresOut, roiOut, batchSplit});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores, bboxDeltas, anchors, imageInfo},
    {scoresOut, roiOut, batchSplit});
  assert(model->isValid());
}

inline bool is_ignored_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type29(Type::TENSOR_FLOAT32, {1, 8, 2, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {2, 4});
  OperandType type4(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type5(Type::TENSOR_FLOAT32, {4});
  OperandType type6(Type::TENSOR_FLOAT32, {4, 4});
  OperandType type7(Type::TENSOR_INT32, {4});
  OperandType type8(Type::FLOAT32, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type1);
  auto bboxDeltas = model->addOperand(&type29);
  auto anchors = model->addOperand(&type3);
  auto imageInfo = model->addOperand(&type4);
  auto param = model->addOperand(&type8);
  auto param1 = model->addOperand(&type8);
  auto param2 = model->addOperand(&type9);
  auto param3 = model->addOperand(&type9);
  auto param4 = model->addOperand(&type8);
  auto param5 = model->addOperand(&type8);
  auto layout = model->addOperand(&type0);
  auto scoresOut = model->addOperand(&type5);
  auto roiOut = model->addOperand(&type6);
  auto batchSplit = model->addOperand(&type7);
  // Phase 2, operations
  static float param_init[] = {4.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  static float param1_init[] = {4.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static int32_t param2_init[] = {-1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {-1};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static float param4_init[] = {0.3f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {1.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GENERATE_PROPOSALS, {scores, bboxDeltas, anchors, imageInfo, param, param1, param2, param3, param4, param5, layout}, {scoresOut, roiOut, batchSplit});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores, bboxDeltas, anchors, imageInfo},
    {scoresOut, roiOut, batchSplit});
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
  OperandType type16(Type::TENSOR_QUANT16_SYMM, {2, 4}, 0.125f, 0);
  OperandType type18(Type::TENSOR_QUANT16_ASYMM, {1, 2}, 0.125f, 0);
  OperandType type19(Type::TENSOR_QUANT16_ASYMM, {4, 4}, 0.125f, 0);
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.01f, 100);
  OperandType type21(Type::TENSOR_QUANT8_ASYMM, {4}, 0.01f, 100);
  OperandType type30(Type::TENSOR_QUANT8_ASYMM, {1, 8, 2, 2}, 0.05f, 128);
  OperandType type7(Type::TENSOR_INT32, {4});
  OperandType type8(Type::FLOAT32, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type20);
  auto bboxDeltas = model->addOperand(&type30);
  auto anchors = model->addOperand(&type16);
  auto imageInfo = model->addOperand(&type18);
  auto param = model->addOperand(&type8);
  auto param1 = model->addOperand(&type8);
  auto param2 = model->addOperand(&type9);
  auto param3 = model->addOperand(&type9);
  auto param4 = model->addOperand(&type8);
  auto param5 = model->addOperand(&type8);
  auto layout = model->addOperand(&type0);
  auto scoresOut = model->addOperand(&type21);
  auto roiOut = model->addOperand(&type19);
  auto batchSplit = model->addOperand(&type7);
  // Phase 2, operations
  static float param_init[] = {4.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  static float param1_init[] = {4.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static int32_t param2_init[] = {-1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {-1};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static float param4_init[] = {0.3f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {1.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GENERATE_PROPOSALS, {scores, bboxDeltas, anchors, imageInfo, param, param1, param2, param3, param4, param5, layout}, {scoresOut, roiOut, batchSplit});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores, bboxDeltas, anchors, imageInfo},
    {scoresOut, roiOut, batchSplit});
  assert(model->isValid());
}

inline bool is_ignored_nchw_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type22(Type::TENSOR_FLOAT16, {2, 4});
  OperandType type24(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type25(Type::FLOAT16, {});
  OperandType type26(Type::TENSOR_FLOAT16, {4, 4});
  OperandType type27(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type28(Type::TENSOR_FLOAT16, {4});
  OperandType type31(Type::TENSOR_FLOAT16, {1, 8, 2, 2});
  OperandType type7(Type::TENSOR_INT32, {4});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type27);
  auto bboxDeltas = model->addOperand(&type31);
  auto anchors = model->addOperand(&type22);
  auto imageInfo = model->addOperand(&type24);
  auto param = model->addOperand(&type25);
  auto param1 = model->addOperand(&type25);
  auto param2 = model->addOperand(&type9);
  auto param3 = model->addOperand(&type9);
  auto param4 = model->addOperand(&type25);
  auto param5 = model->addOperand(&type25);
  auto layout = model->addOperand(&type0);
  auto scoresOut = model->addOperand(&type28);
  auto roiOut = model->addOperand(&type26);
  auto batchSplit = model->addOperand(&type7);
  // Phase 2, operations
  static _Float16 param_init[] = {4.0f};
  model->setOperandValue(param, param_init, sizeof(_Float16) * 1);
  static _Float16 param1_init[] = {4.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static int32_t param2_init[] = {-1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {-1};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static _Float16 param4_init[] = {0.30000001192092896f};
  model->setOperandValue(param4, param4_init, sizeof(_Float16) * 1);
  static _Float16 param5_init[] = {1.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GENERATE_PROPOSALS, {scores, bboxDeltas, anchors, imageInfo, param, param1, param2, param3, param4, param5, layout}, {scoresOut, roiOut, batchSplit});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores, bboxDeltas, anchors, imageInfo},
    {scoresOut, roiOut, batchSplit});
  assert(model->isValid());
}

inline bool is_ignored_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 8});
  OperandType type3(Type::TENSOR_FLOAT32, {2, 4});
  OperandType type32(Type::TENSOR_FLOAT32, {0});
  OperandType type33(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type34(Type::TENSOR_INT32, {0});
  OperandType type4(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::FLOAT32, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type1);
  auto bboxDeltas = model->addOperand(&type2);
  auto anchors = model->addOperand(&type3);
  auto imageInfo = model->addOperand(&type4);
  auto param = model->addOperand(&type8);
  auto param1 = model->addOperand(&type8);
  auto param2 = model->addOperand(&type9);
  auto param3 = model->addOperand(&type9);
  auto param4 = model->addOperand(&type8);
  auto param5 = model->addOperand(&type8);
  auto layout = model->addOperand(&type0);
  auto scoresOut = model->addOperand(&type32);
  auto roiOut = model->addOperand(&type33);
  auto batchSplit = model->addOperand(&type34);
  // Phase 2, operations
  static float param_init[] = {4.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  static float param1_init[] = {4.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static int32_t param2_init[] = {-1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {-1};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static float param4_init[] = {0.3f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {1.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GENERATE_PROPOSALS, {scores, bboxDeltas, anchors, imageInfo, param, param1, param2, param3, param4, param5, layout}, {scoresOut, roiOut, batchSplit});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores, bboxDeltas, anchors, imageInfo},
    {scoresOut, roiOut, batchSplit});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 8});
  OperandType type3(Type::TENSOR_FLOAT32, {2, 4});
  OperandType type32(Type::TENSOR_FLOAT32, {0});
  OperandType type33(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type34(Type::TENSOR_INT32, {0});
  OperandType type4(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::FLOAT32, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type1);
  auto bboxDeltas = model->addOperand(&type2);
  auto anchors = model->addOperand(&type3);
  auto imageInfo = model->addOperand(&type4);
  auto param = model->addOperand(&type8);
  auto param1 = model->addOperand(&type8);
  auto param2 = model->addOperand(&type9);
  auto param3 = model->addOperand(&type9);
  auto param4 = model->addOperand(&type8);
  auto param5 = model->addOperand(&type8);
  auto layout = model->addOperand(&type0);
  auto scoresOut = model->addOperand(&type32);
  auto roiOut = model->addOperand(&type33);
  auto batchSplit = model->addOperand(&type34);
  // Phase 2, operations
  static float param_init[] = {4.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  static float param1_init[] = {4.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static int32_t param2_init[] = {-1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {-1};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static float param4_init[] = {0.3f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {1.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GENERATE_PROPOSALS, {scores, bboxDeltas, anchors, imageInfo, param, param1, param2, param3, param4, param5, layout}, {scoresOut, roiOut, batchSplit});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores, bboxDeltas, anchors, imageInfo},
    {scoresOut, roiOut, batchSplit});
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
  OperandType type16(Type::TENSOR_QUANT16_SYMM, {2, 4}, 0.125f, 0);
  OperandType type17(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 8}, 0.05f, 128);
  OperandType type18(Type::TENSOR_QUANT16_ASYMM, {1, 2}, 0.125f, 0);
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.01f, 100);
  OperandType type34(Type::TENSOR_INT32, {0});
  OperandType type35(Type::TENSOR_QUANT8_ASYMM, {0}, 0.01f, 100);
  OperandType type36(Type::TENSOR_QUANT16_ASYMM, {0, 0}, 0.125f, 0);
  OperandType type8(Type::FLOAT32, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type20);
  auto bboxDeltas = model->addOperand(&type17);
  auto anchors = model->addOperand(&type16);
  auto imageInfo = model->addOperand(&type18);
  auto param = model->addOperand(&type8);
  auto param1 = model->addOperand(&type8);
  auto param2 = model->addOperand(&type9);
  auto param3 = model->addOperand(&type9);
  auto param4 = model->addOperand(&type8);
  auto param5 = model->addOperand(&type8);
  auto layout = model->addOperand(&type0);
  auto scoresOut = model->addOperand(&type35);
  auto roiOut = model->addOperand(&type36);
  auto batchSplit = model->addOperand(&type34);
  // Phase 2, operations
  static float param_init[] = {4.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  static float param1_init[] = {4.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static int32_t param2_init[] = {-1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {-1};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static float param4_init[] = {0.3f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {1.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GENERATE_PROPOSALS, {scores, bboxDeltas, anchors, imageInfo, param, param1, param2, param3, param4, param5, layout}, {scoresOut, roiOut, batchSplit});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores, bboxDeltas, anchors, imageInfo},
    {scoresOut, roiOut, batchSplit});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type22(Type::TENSOR_FLOAT16, {2, 4});
  OperandType type23(Type::TENSOR_FLOAT16, {1, 2, 2, 8});
  OperandType type24(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type25(Type::FLOAT16, {});
  OperandType type27(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type34(Type::TENSOR_INT32, {0});
  OperandType type37(Type::TENSOR_FLOAT16, {0});
  OperandType type38(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type27);
  auto bboxDeltas = model->addOperand(&type23);
  auto anchors = model->addOperand(&type22);
  auto imageInfo = model->addOperand(&type24);
  auto param = model->addOperand(&type25);
  auto param1 = model->addOperand(&type25);
  auto param2 = model->addOperand(&type9);
  auto param3 = model->addOperand(&type9);
  auto param4 = model->addOperand(&type25);
  auto param5 = model->addOperand(&type25);
  auto layout = model->addOperand(&type0);
  auto scoresOut = model->addOperand(&type37);
  auto roiOut = model->addOperand(&type38);
  auto batchSplit = model->addOperand(&type34);
  // Phase 2, operations
  static _Float16 param_init[] = {4.0f};
  model->setOperandValue(param, param_init, sizeof(_Float16) * 1);
  static _Float16 param1_init[] = {4.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static int32_t param2_init[] = {-1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {-1};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static _Float16 param4_init[] = {0.30000001192092896f};
  model->setOperandValue(param4, param4_init, sizeof(_Float16) * 1);
  static _Float16 param5_init[] = {1.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GENERATE_PROPOSALS, {scores, bboxDeltas, anchors, imageInfo, param, param1, param2, param3, param4, param5, layout}, {scoresOut, roiOut, batchSplit});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores, bboxDeltas, anchors, imageInfo},
    {scoresOut, roiOut, batchSplit});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type29(Type::TENSOR_FLOAT32, {1, 8, 2, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {2, 4});
  OperandType type32(Type::TENSOR_FLOAT32, {0});
  OperandType type33(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type34(Type::TENSOR_INT32, {0});
  OperandType type4(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::FLOAT32, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type1);
  auto bboxDeltas = model->addOperand(&type29);
  auto anchors = model->addOperand(&type3);
  auto imageInfo = model->addOperand(&type4);
  auto param = model->addOperand(&type8);
  auto param1 = model->addOperand(&type8);
  auto param2 = model->addOperand(&type9);
  auto param3 = model->addOperand(&type9);
  auto param4 = model->addOperand(&type8);
  auto param5 = model->addOperand(&type8);
  auto layout = model->addOperand(&type0);
  auto scoresOut = model->addOperand(&type32);
  auto roiOut = model->addOperand(&type33);
  auto batchSplit = model->addOperand(&type34);
  // Phase 2, operations
  static float param_init[] = {4.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  static float param1_init[] = {4.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static int32_t param2_init[] = {-1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {-1};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static float param4_init[] = {0.3f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {1.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GENERATE_PROPOSALS, {scores, bboxDeltas, anchors, imageInfo, param, param1, param2, param3, param4, param5, layout}, {scoresOut, roiOut, batchSplit});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores, bboxDeltas, anchors, imageInfo},
    {scoresOut, roiOut, batchSplit});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type29(Type::TENSOR_FLOAT32, {1, 8, 2, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {2, 4});
  OperandType type32(Type::TENSOR_FLOAT32, {0});
  OperandType type33(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type34(Type::TENSOR_INT32, {0});
  OperandType type4(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::FLOAT32, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type1);
  auto bboxDeltas = model->addOperand(&type29);
  auto anchors = model->addOperand(&type3);
  auto imageInfo = model->addOperand(&type4);
  auto param = model->addOperand(&type8);
  auto param1 = model->addOperand(&type8);
  auto param2 = model->addOperand(&type9);
  auto param3 = model->addOperand(&type9);
  auto param4 = model->addOperand(&type8);
  auto param5 = model->addOperand(&type8);
  auto layout = model->addOperand(&type0);
  auto scoresOut = model->addOperand(&type32);
  auto roiOut = model->addOperand(&type33);
  auto batchSplit = model->addOperand(&type34);
  // Phase 2, operations
  static float param_init[] = {4.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  static float param1_init[] = {4.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static int32_t param2_init[] = {-1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {-1};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static float param4_init[] = {0.3f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {1.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GENERATE_PROPOSALS, {scores, bboxDeltas, anchors, imageInfo, param, param1, param2, param3, param4, param5, layout}, {scoresOut, roiOut, batchSplit});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores, bboxDeltas, anchors, imageInfo},
    {scoresOut, roiOut, batchSplit});
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
  OperandType type16(Type::TENSOR_QUANT16_SYMM, {2, 4}, 0.125f, 0);
  OperandType type18(Type::TENSOR_QUANT16_ASYMM, {1, 2}, 0.125f, 0);
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.01f, 100);
  OperandType type30(Type::TENSOR_QUANT8_ASYMM, {1, 8, 2, 2}, 0.05f, 128);
  OperandType type34(Type::TENSOR_INT32, {0});
  OperandType type35(Type::TENSOR_QUANT8_ASYMM, {0}, 0.01f, 100);
  OperandType type36(Type::TENSOR_QUANT16_ASYMM, {0, 0}, 0.125f, 0);
  OperandType type8(Type::FLOAT32, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type20);
  auto bboxDeltas = model->addOperand(&type30);
  auto anchors = model->addOperand(&type16);
  auto imageInfo = model->addOperand(&type18);
  auto param = model->addOperand(&type8);
  auto param1 = model->addOperand(&type8);
  auto param2 = model->addOperand(&type9);
  auto param3 = model->addOperand(&type9);
  auto param4 = model->addOperand(&type8);
  auto param5 = model->addOperand(&type8);
  auto layout = model->addOperand(&type0);
  auto scoresOut = model->addOperand(&type35);
  auto roiOut = model->addOperand(&type36);
  auto batchSplit = model->addOperand(&type34);
  // Phase 2, operations
  static float param_init[] = {4.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  static float param1_init[] = {4.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static int32_t param2_init[] = {-1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {-1};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static float param4_init[] = {0.3f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {1.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GENERATE_PROPOSALS, {scores, bboxDeltas, anchors, imageInfo, param, param1, param2, param3, param4, param5, layout}, {scoresOut, roiOut, batchSplit});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores, bboxDeltas, anchors, imageInfo},
    {scoresOut, roiOut, batchSplit});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type22(Type::TENSOR_FLOAT16, {2, 4});
  OperandType type24(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type25(Type::FLOAT16, {});
  OperandType type27(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type31(Type::TENSOR_FLOAT16, {1, 8, 2, 2});
  OperandType type34(Type::TENSOR_INT32, {0});
  OperandType type37(Type::TENSOR_FLOAT16, {0});
  OperandType type38(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type27);
  auto bboxDeltas = model->addOperand(&type31);
  auto anchors = model->addOperand(&type22);
  auto imageInfo = model->addOperand(&type24);
  auto param = model->addOperand(&type25);
  auto param1 = model->addOperand(&type25);
  auto param2 = model->addOperand(&type9);
  auto param3 = model->addOperand(&type9);
  auto param4 = model->addOperand(&type25);
  auto param5 = model->addOperand(&type25);
  auto layout = model->addOperand(&type0);
  auto scoresOut = model->addOperand(&type37);
  auto roiOut = model->addOperand(&type38);
  auto batchSplit = model->addOperand(&type34);
  // Phase 2, operations
  static _Float16 param_init[] = {4.0f};
  model->setOperandValue(param, param_init, sizeof(_Float16) * 1);
  static _Float16 param1_init[] = {4.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static int32_t param2_init[] = {-1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {-1};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static _Float16 param4_init[] = {0.30000001192092896f};
  model->setOperandValue(param4, param4_init, sizeof(_Float16) * 1);
  static _Float16 param5_init[] = {1.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GENERATE_PROPOSALS, {scores, bboxDeltas, anchors, imageInfo, param, param1, param2, param3, param4, param5, layout}, {scoresOut, roiOut, batchSplit});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores, bboxDeltas, anchors, imageInfo},
    {scoresOut, roiOut, batchSplit});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {2, 4, 4, 4});
  OperandType type11(Type::TENSOR_FLOAT32, {2, 4, 4, 16});
  OperandType type12(Type::TENSOR_FLOAT32, {2, 2});
  OperandType type13(Type::TENSOR_FLOAT32, {30});
  OperandType type14(Type::TENSOR_FLOAT32, {30, 4});
  OperandType type15(Type::TENSOR_INT32, {30});
  OperandType type6(Type::TENSOR_FLOAT32, {4, 4});
  OperandType type8(Type::FLOAT32, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type10);
  auto bboxDeltas1 = model->addOperand(&type11);
  auto anchors1 = model->addOperand(&type6);
  auto imageInfo1 = model->addOperand(&type12);
  auto param6 = model->addOperand(&type8);
  auto param7 = model->addOperand(&type8);
  auto param8 = model->addOperand(&type9);
  auto param9 = model->addOperand(&type9);
  auto param10 = model->addOperand(&type8);
  auto param11 = model->addOperand(&type8);
  auto layout = model->addOperand(&type0);
  auto scoresOut1 = model->addOperand(&type13);
  auto roiOut1 = model->addOperand(&type14);
  auto batchSplit1 = model->addOperand(&type15);
  // Phase 2, operations
  static float param6_init[] = {10.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {10.0f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t param8_init[] = {32};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {16};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static float param10_init[] = {0.2f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {1.0f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GENERATE_PROPOSALS, {scores1, bboxDeltas1, anchors1, imageInfo1, param6, param7, param8, param9, param10, param11, layout}, {scoresOut1, roiOut1, batchSplit1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores1, bboxDeltas1, anchors1, imageInfo1},
    {scoresOut1, roiOut1, batchSplit1});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {2, 4, 4, 4});
  OperandType type11(Type::TENSOR_FLOAT32, {2, 4, 4, 16});
  OperandType type12(Type::TENSOR_FLOAT32, {2, 2});
  OperandType type13(Type::TENSOR_FLOAT32, {30});
  OperandType type14(Type::TENSOR_FLOAT32, {30, 4});
  OperandType type15(Type::TENSOR_INT32, {30});
  OperandType type6(Type::TENSOR_FLOAT32, {4, 4});
  OperandType type8(Type::FLOAT32, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type10);
  auto bboxDeltas1 = model->addOperand(&type11);
  auto anchors1 = model->addOperand(&type6);
  auto imageInfo1 = model->addOperand(&type12);
  auto param6 = model->addOperand(&type8);
  auto param7 = model->addOperand(&type8);
  auto param8 = model->addOperand(&type9);
  auto param9 = model->addOperand(&type9);
  auto param10 = model->addOperand(&type8);
  auto param11 = model->addOperand(&type8);
  auto layout = model->addOperand(&type0);
  auto scoresOut1 = model->addOperand(&type13);
  auto roiOut1 = model->addOperand(&type14);
  auto batchSplit1 = model->addOperand(&type15);
  // Phase 2, operations
  static float param6_init[] = {10.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {10.0f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t param8_init[] = {32};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {16};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static float param10_init[] = {0.2f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {1.0f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GENERATE_PROPOSALS, {scores1, bboxDeltas1, anchors1, imageInfo1, param6, param7, param8, param9, param10, param11, layout}, {scoresOut1, roiOut1, batchSplit1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores1, bboxDeltas1, anchors1, imageInfo1},
    {scoresOut1, roiOut1, batchSplit1});
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
  OperandType type15(Type::TENSOR_INT32, {30});
  OperandType type39(Type::TENSOR_QUANT16_SYMM, {4, 4}, 0.125f, 0);
  OperandType type40(Type::TENSOR_QUANT8_ASYMM, {2, 4, 4, 16}, 0.1f, 128);
  OperandType type41(Type::TENSOR_QUANT16_ASYMM, {2, 2}, 0.125f, 0);
  OperandType type42(Type::TENSOR_QUANT16_ASYMM, {30, 4}, 0.125f, 0);
  OperandType type43(Type::TENSOR_QUANT8_ASYMM, {2, 4, 4, 4}, 0.005f, 0);
  OperandType type44(Type::TENSOR_QUANT8_ASYMM, {30}, 0.005f, 0);
  OperandType type8(Type::FLOAT32, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type43);
  auto bboxDeltas1 = model->addOperand(&type40);
  auto anchors1 = model->addOperand(&type39);
  auto imageInfo1 = model->addOperand(&type41);
  auto param6 = model->addOperand(&type8);
  auto param7 = model->addOperand(&type8);
  auto param8 = model->addOperand(&type9);
  auto param9 = model->addOperand(&type9);
  auto param10 = model->addOperand(&type8);
  auto param11 = model->addOperand(&type8);
  auto layout = model->addOperand(&type0);
  auto scoresOut1 = model->addOperand(&type44);
  auto roiOut1 = model->addOperand(&type42);
  auto batchSplit1 = model->addOperand(&type15);
  // Phase 2, operations
  static float param6_init[] = {10.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {10.0f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t param8_init[] = {32};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {16};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static float param10_init[] = {0.2f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {1.0f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GENERATE_PROPOSALS, {scores1, bboxDeltas1, anchors1, imageInfo1, param6, param7, param8, param9, param10, param11, layout}, {scoresOut1, roiOut1, batchSplit1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores1, bboxDeltas1, anchors1, imageInfo1},
    {scoresOut1, roiOut1, batchSplit1});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type15(Type::TENSOR_INT32, {30});
  OperandType type25(Type::FLOAT16, {});
  OperandType type26(Type::TENSOR_FLOAT16, {4, 4});
  OperandType type45(Type::TENSOR_FLOAT16, {2, 4, 4, 16});
  OperandType type46(Type::TENSOR_FLOAT16, {2, 2});
  OperandType type47(Type::TENSOR_FLOAT16, {30, 4});
  OperandType type48(Type::TENSOR_FLOAT16, {2, 4, 4, 4});
  OperandType type49(Type::TENSOR_FLOAT16, {30});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type48);
  auto bboxDeltas1 = model->addOperand(&type45);
  auto anchors1 = model->addOperand(&type26);
  auto imageInfo1 = model->addOperand(&type46);
  auto param6 = model->addOperand(&type25);
  auto param7 = model->addOperand(&type25);
  auto param8 = model->addOperand(&type9);
  auto param9 = model->addOperand(&type9);
  auto param10 = model->addOperand(&type25);
  auto param11 = model->addOperand(&type25);
  auto layout = model->addOperand(&type0);
  auto scoresOut1 = model->addOperand(&type49);
  auto roiOut1 = model->addOperand(&type47);
  auto batchSplit1 = model->addOperand(&type15);
  // Phase 2, operations
  static _Float16 param6_init[] = {10.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {10.0f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t param8_init[] = {32};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {16};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static _Float16 param10_init[] = {0.20000000298023224f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {1.0f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GENERATE_PROPOSALS, {scores1, bboxDeltas1, anchors1, imageInfo1, param6, param7, param8, param9, param10, param11, layout}, {scoresOut1, roiOut1, batchSplit1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores1, bboxDeltas1, anchors1, imageInfo1},
    {scoresOut1, roiOut1, batchSplit1});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {2, 4, 4, 4});
  OperandType type12(Type::TENSOR_FLOAT32, {2, 2});
  OperandType type13(Type::TENSOR_FLOAT32, {30});
  OperandType type14(Type::TENSOR_FLOAT32, {30, 4});
  OperandType type15(Type::TENSOR_INT32, {30});
  OperandType type50(Type::TENSOR_FLOAT32, {2, 16, 4, 4});
  OperandType type6(Type::TENSOR_FLOAT32, {4, 4});
  OperandType type8(Type::FLOAT32, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type10);
  auto bboxDeltas1 = model->addOperand(&type50);
  auto anchors1 = model->addOperand(&type6);
  auto imageInfo1 = model->addOperand(&type12);
  auto param6 = model->addOperand(&type8);
  auto param7 = model->addOperand(&type8);
  auto param8 = model->addOperand(&type9);
  auto param9 = model->addOperand(&type9);
  auto param10 = model->addOperand(&type8);
  auto param11 = model->addOperand(&type8);
  auto layout = model->addOperand(&type0);
  auto scoresOut1 = model->addOperand(&type13);
  auto roiOut1 = model->addOperand(&type14);
  auto batchSplit1 = model->addOperand(&type15);
  // Phase 2, operations
  static float param6_init[] = {10.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {10.0f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t param8_init[] = {32};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {16};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static float param10_init[] = {0.2f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {1.0f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GENERATE_PROPOSALS, {scores1, bboxDeltas1, anchors1, imageInfo1, param6, param7, param8, param9, param10, param11, layout}, {scoresOut1, roiOut1, batchSplit1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores1, bboxDeltas1, anchors1, imageInfo1},
    {scoresOut1, roiOut1, batchSplit1});
  assert(model->isValid());
}

inline bool is_ignored_nchw_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {2, 4, 4, 4});
  OperandType type12(Type::TENSOR_FLOAT32, {2, 2});
  OperandType type13(Type::TENSOR_FLOAT32, {30});
  OperandType type14(Type::TENSOR_FLOAT32, {30, 4});
  OperandType type15(Type::TENSOR_INT32, {30});
  OperandType type50(Type::TENSOR_FLOAT32, {2, 16, 4, 4});
  OperandType type6(Type::TENSOR_FLOAT32, {4, 4});
  OperandType type8(Type::FLOAT32, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type10);
  auto bboxDeltas1 = model->addOperand(&type50);
  auto anchors1 = model->addOperand(&type6);
  auto imageInfo1 = model->addOperand(&type12);
  auto param6 = model->addOperand(&type8);
  auto param7 = model->addOperand(&type8);
  auto param8 = model->addOperand(&type9);
  auto param9 = model->addOperand(&type9);
  auto param10 = model->addOperand(&type8);
  auto param11 = model->addOperand(&type8);
  auto layout = model->addOperand(&type0);
  auto scoresOut1 = model->addOperand(&type13);
  auto roiOut1 = model->addOperand(&type14);
  auto batchSplit1 = model->addOperand(&type15);
  // Phase 2, operations
  static float param6_init[] = {10.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {10.0f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t param8_init[] = {32};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {16};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static float param10_init[] = {0.2f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {1.0f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GENERATE_PROPOSALS, {scores1, bboxDeltas1, anchors1, imageInfo1, param6, param7, param8, param9, param10, param11, layout}, {scoresOut1, roiOut1, batchSplit1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores1, bboxDeltas1, anchors1, imageInfo1},
    {scoresOut1, roiOut1, batchSplit1});
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
  OperandType type15(Type::TENSOR_INT32, {30});
  OperandType type39(Type::TENSOR_QUANT16_SYMM, {4, 4}, 0.125f, 0);
  OperandType type41(Type::TENSOR_QUANT16_ASYMM, {2, 2}, 0.125f, 0);
  OperandType type42(Type::TENSOR_QUANT16_ASYMM, {30, 4}, 0.125f, 0);
  OperandType type43(Type::TENSOR_QUANT8_ASYMM, {2, 4, 4, 4}, 0.005f, 0);
  OperandType type44(Type::TENSOR_QUANT8_ASYMM, {30}, 0.005f, 0);
  OperandType type51(Type::TENSOR_QUANT8_ASYMM, {2, 16, 4, 4}, 0.1f, 128);
  OperandType type8(Type::FLOAT32, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type43);
  auto bboxDeltas1 = model->addOperand(&type51);
  auto anchors1 = model->addOperand(&type39);
  auto imageInfo1 = model->addOperand(&type41);
  auto param6 = model->addOperand(&type8);
  auto param7 = model->addOperand(&type8);
  auto param8 = model->addOperand(&type9);
  auto param9 = model->addOperand(&type9);
  auto param10 = model->addOperand(&type8);
  auto param11 = model->addOperand(&type8);
  auto layout = model->addOperand(&type0);
  auto scoresOut1 = model->addOperand(&type44);
  auto roiOut1 = model->addOperand(&type42);
  auto batchSplit1 = model->addOperand(&type15);
  // Phase 2, operations
  static float param6_init[] = {10.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {10.0f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t param8_init[] = {32};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {16};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static float param10_init[] = {0.2f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {1.0f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GENERATE_PROPOSALS, {scores1, bboxDeltas1, anchors1, imageInfo1, param6, param7, param8, param9, param10, param11, layout}, {scoresOut1, roiOut1, batchSplit1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores1, bboxDeltas1, anchors1, imageInfo1},
    {scoresOut1, roiOut1, batchSplit1});
  assert(model->isValid());
}

inline bool is_ignored_nchw_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type15(Type::TENSOR_INT32, {30});
  OperandType type25(Type::FLOAT16, {});
  OperandType type26(Type::TENSOR_FLOAT16, {4, 4});
  OperandType type46(Type::TENSOR_FLOAT16, {2, 2});
  OperandType type47(Type::TENSOR_FLOAT16, {30, 4});
  OperandType type48(Type::TENSOR_FLOAT16, {2, 4, 4, 4});
  OperandType type49(Type::TENSOR_FLOAT16, {30});
  OperandType type52(Type::TENSOR_FLOAT16, {2, 16, 4, 4});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type48);
  auto bboxDeltas1 = model->addOperand(&type52);
  auto anchors1 = model->addOperand(&type26);
  auto imageInfo1 = model->addOperand(&type46);
  auto param6 = model->addOperand(&type25);
  auto param7 = model->addOperand(&type25);
  auto param8 = model->addOperand(&type9);
  auto param9 = model->addOperand(&type9);
  auto param10 = model->addOperand(&type25);
  auto param11 = model->addOperand(&type25);
  auto layout = model->addOperand(&type0);
  auto scoresOut1 = model->addOperand(&type49);
  auto roiOut1 = model->addOperand(&type47);
  auto batchSplit1 = model->addOperand(&type15);
  // Phase 2, operations
  static _Float16 param6_init[] = {10.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {10.0f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t param8_init[] = {32};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {16};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static _Float16 param10_init[] = {0.20000000298023224f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {1.0f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GENERATE_PROPOSALS, {scores1, bboxDeltas1, anchors1, imageInfo1, param6, param7, param8, param9, param10, param11, layout}, {scoresOut1, roiOut1, batchSplit1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores1, bboxDeltas1, anchors1, imageInfo1},
    {scoresOut1, roiOut1, batchSplit1});
  assert(model->isValid());
}

inline bool is_ignored_nchw_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {2, 4, 4, 4});
  OperandType type11(Type::TENSOR_FLOAT32, {2, 4, 4, 16});
  OperandType type12(Type::TENSOR_FLOAT32, {2, 2});
  OperandType type32(Type::TENSOR_FLOAT32, {0});
  OperandType type33(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type34(Type::TENSOR_INT32, {0});
  OperandType type6(Type::TENSOR_FLOAT32, {4, 4});
  OperandType type8(Type::FLOAT32, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type10);
  auto bboxDeltas1 = model->addOperand(&type11);
  auto anchors1 = model->addOperand(&type6);
  auto imageInfo1 = model->addOperand(&type12);
  auto param6 = model->addOperand(&type8);
  auto param7 = model->addOperand(&type8);
  auto param8 = model->addOperand(&type9);
  auto param9 = model->addOperand(&type9);
  auto param10 = model->addOperand(&type8);
  auto param11 = model->addOperand(&type8);
  auto layout = model->addOperand(&type0);
  auto scoresOut1 = model->addOperand(&type32);
  auto roiOut1 = model->addOperand(&type33);
  auto batchSplit1 = model->addOperand(&type34);
  // Phase 2, operations
  static float param6_init[] = {10.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {10.0f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t param8_init[] = {32};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {16};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static float param10_init[] = {0.2f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {1.0f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GENERATE_PROPOSALS, {scores1, bboxDeltas1, anchors1, imageInfo1, param6, param7, param8, param9, param10, param11, layout}, {scoresOut1, roiOut1, batchSplit1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores1, bboxDeltas1, anchors1, imageInfo1},
    {scoresOut1, roiOut1, batchSplit1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {2, 4, 4, 4});
  OperandType type11(Type::TENSOR_FLOAT32, {2, 4, 4, 16});
  OperandType type12(Type::TENSOR_FLOAT32, {2, 2});
  OperandType type32(Type::TENSOR_FLOAT32, {0});
  OperandType type33(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type34(Type::TENSOR_INT32, {0});
  OperandType type6(Type::TENSOR_FLOAT32, {4, 4});
  OperandType type8(Type::FLOAT32, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type10);
  auto bboxDeltas1 = model->addOperand(&type11);
  auto anchors1 = model->addOperand(&type6);
  auto imageInfo1 = model->addOperand(&type12);
  auto param6 = model->addOperand(&type8);
  auto param7 = model->addOperand(&type8);
  auto param8 = model->addOperand(&type9);
  auto param9 = model->addOperand(&type9);
  auto param10 = model->addOperand(&type8);
  auto param11 = model->addOperand(&type8);
  auto layout = model->addOperand(&type0);
  auto scoresOut1 = model->addOperand(&type32);
  auto roiOut1 = model->addOperand(&type33);
  auto batchSplit1 = model->addOperand(&type34);
  // Phase 2, operations
  static float param6_init[] = {10.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {10.0f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t param8_init[] = {32};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {16};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static float param10_init[] = {0.2f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {1.0f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GENERATE_PROPOSALS, {scores1, bboxDeltas1, anchors1, imageInfo1, param6, param7, param8, param9, param10, param11, layout}, {scoresOut1, roiOut1, batchSplit1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores1, bboxDeltas1, anchors1, imageInfo1},
    {scoresOut1, roiOut1, batchSplit1});
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
  OperandType type34(Type::TENSOR_INT32, {0});
  OperandType type36(Type::TENSOR_QUANT16_ASYMM, {0, 0}, 0.125f, 0);
  OperandType type39(Type::TENSOR_QUANT16_SYMM, {4, 4}, 0.125f, 0);
  OperandType type40(Type::TENSOR_QUANT8_ASYMM, {2, 4, 4, 16}, 0.1f, 128);
  OperandType type41(Type::TENSOR_QUANT16_ASYMM, {2, 2}, 0.125f, 0);
  OperandType type43(Type::TENSOR_QUANT8_ASYMM, {2, 4, 4, 4}, 0.005f, 0);
  OperandType type53(Type::TENSOR_QUANT8_ASYMM, {0}, 0.005f, 0);
  OperandType type8(Type::FLOAT32, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type43);
  auto bboxDeltas1 = model->addOperand(&type40);
  auto anchors1 = model->addOperand(&type39);
  auto imageInfo1 = model->addOperand(&type41);
  auto param6 = model->addOperand(&type8);
  auto param7 = model->addOperand(&type8);
  auto param8 = model->addOperand(&type9);
  auto param9 = model->addOperand(&type9);
  auto param10 = model->addOperand(&type8);
  auto param11 = model->addOperand(&type8);
  auto layout = model->addOperand(&type0);
  auto scoresOut1 = model->addOperand(&type53);
  auto roiOut1 = model->addOperand(&type36);
  auto batchSplit1 = model->addOperand(&type34);
  // Phase 2, operations
  static float param6_init[] = {10.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {10.0f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t param8_init[] = {32};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {16};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static float param10_init[] = {0.2f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {1.0f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GENERATE_PROPOSALS, {scores1, bboxDeltas1, anchors1, imageInfo1, param6, param7, param8, param9, param10, param11, layout}, {scoresOut1, roiOut1, batchSplit1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores1, bboxDeltas1, anchors1, imageInfo1},
    {scoresOut1, roiOut1, batchSplit1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type25(Type::FLOAT16, {});
  OperandType type26(Type::TENSOR_FLOAT16, {4, 4});
  OperandType type34(Type::TENSOR_INT32, {0});
  OperandType type37(Type::TENSOR_FLOAT16, {0});
  OperandType type38(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type45(Type::TENSOR_FLOAT16, {2, 4, 4, 16});
  OperandType type46(Type::TENSOR_FLOAT16, {2, 2});
  OperandType type48(Type::TENSOR_FLOAT16, {2, 4, 4, 4});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type48);
  auto bboxDeltas1 = model->addOperand(&type45);
  auto anchors1 = model->addOperand(&type26);
  auto imageInfo1 = model->addOperand(&type46);
  auto param6 = model->addOperand(&type25);
  auto param7 = model->addOperand(&type25);
  auto param8 = model->addOperand(&type9);
  auto param9 = model->addOperand(&type9);
  auto param10 = model->addOperand(&type25);
  auto param11 = model->addOperand(&type25);
  auto layout = model->addOperand(&type0);
  auto scoresOut1 = model->addOperand(&type37);
  auto roiOut1 = model->addOperand(&type38);
  auto batchSplit1 = model->addOperand(&type34);
  // Phase 2, operations
  static _Float16 param6_init[] = {10.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {10.0f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t param8_init[] = {32};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {16};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static _Float16 param10_init[] = {0.20000000298023224f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {1.0f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GENERATE_PROPOSALS, {scores1, bboxDeltas1, anchors1, imageInfo1, param6, param7, param8, param9, param10, param11, layout}, {scoresOut1, roiOut1, batchSplit1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores1, bboxDeltas1, anchors1, imageInfo1},
    {scoresOut1, roiOut1, batchSplit1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {2, 4, 4, 4});
  OperandType type12(Type::TENSOR_FLOAT32, {2, 2});
  OperandType type32(Type::TENSOR_FLOAT32, {0});
  OperandType type33(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type34(Type::TENSOR_INT32, {0});
  OperandType type50(Type::TENSOR_FLOAT32, {2, 16, 4, 4});
  OperandType type6(Type::TENSOR_FLOAT32, {4, 4});
  OperandType type8(Type::FLOAT32, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type10);
  auto bboxDeltas1 = model->addOperand(&type50);
  auto anchors1 = model->addOperand(&type6);
  auto imageInfo1 = model->addOperand(&type12);
  auto param6 = model->addOperand(&type8);
  auto param7 = model->addOperand(&type8);
  auto param8 = model->addOperand(&type9);
  auto param9 = model->addOperand(&type9);
  auto param10 = model->addOperand(&type8);
  auto param11 = model->addOperand(&type8);
  auto layout = model->addOperand(&type0);
  auto scoresOut1 = model->addOperand(&type32);
  auto roiOut1 = model->addOperand(&type33);
  auto batchSplit1 = model->addOperand(&type34);
  // Phase 2, operations
  static float param6_init[] = {10.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {10.0f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t param8_init[] = {32};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {16};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static float param10_init[] = {0.2f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {1.0f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GENERATE_PROPOSALS, {scores1, bboxDeltas1, anchors1, imageInfo1, param6, param7, param8, param9, param10, param11, layout}, {scoresOut1, roiOut1, batchSplit1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores1, bboxDeltas1, anchors1, imageInfo1},
    {scoresOut1, roiOut1, batchSplit1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {2, 4, 4, 4});
  OperandType type12(Type::TENSOR_FLOAT32, {2, 2});
  OperandType type32(Type::TENSOR_FLOAT32, {0});
  OperandType type33(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type34(Type::TENSOR_INT32, {0});
  OperandType type50(Type::TENSOR_FLOAT32, {2, 16, 4, 4});
  OperandType type6(Type::TENSOR_FLOAT32, {4, 4});
  OperandType type8(Type::FLOAT32, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type10);
  auto bboxDeltas1 = model->addOperand(&type50);
  auto anchors1 = model->addOperand(&type6);
  auto imageInfo1 = model->addOperand(&type12);
  auto param6 = model->addOperand(&type8);
  auto param7 = model->addOperand(&type8);
  auto param8 = model->addOperand(&type9);
  auto param9 = model->addOperand(&type9);
  auto param10 = model->addOperand(&type8);
  auto param11 = model->addOperand(&type8);
  auto layout = model->addOperand(&type0);
  auto scoresOut1 = model->addOperand(&type32);
  auto roiOut1 = model->addOperand(&type33);
  auto batchSplit1 = model->addOperand(&type34);
  // Phase 2, operations
  static float param6_init[] = {10.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {10.0f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t param8_init[] = {32};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {16};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static float param10_init[] = {0.2f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {1.0f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GENERATE_PROPOSALS, {scores1, bboxDeltas1, anchors1, imageInfo1, param6, param7, param8, param9, param10, param11, layout}, {scoresOut1, roiOut1, batchSplit1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores1, bboxDeltas1, anchors1, imageInfo1},
    {scoresOut1, roiOut1, batchSplit1});
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
  OperandType type34(Type::TENSOR_INT32, {0});
  OperandType type36(Type::TENSOR_QUANT16_ASYMM, {0, 0}, 0.125f, 0);
  OperandType type39(Type::TENSOR_QUANT16_SYMM, {4, 4}, 0.125f, 0);
  OperandType type41(Type::TENSOR_QUANT16_ASYMM, {2, 2}, 0.125f, 0);
  OperandType type43(Type::TENSOR_QUANT8_ASYMM, {2, 4, 4, 4}, 0.005f, 0);
  OperandType type51(Type::TENSOR_QUANT8_ASYMM, {2, 16, 4, 4}, 0.1f, 128);
  OperandType type53(Type::TENSOR_QUANT8_ASYMM, {0}, 0.005f, 0);
  OperandType type8(Type::FLOAT32, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type43);
  auto bboxDeltas1 = model->addOperand(&type51);
  auto anchors1 = model->addOperand(&type39);
  auto imageInfo1 = model->addOperand(&type41);
  auto param6 = model->addOperand(&type8);
  auto param7 = model->addOperand(&type8);
  auto param8 = model->addOperand(&type9);
  auto param9 = model->addOperand(&type9);
  auto param10 = model->addOperand(&type8);
  auto param11 = model->addOperand(&type8);
  auto layout = model->addOperand(&type0);
  auto scoresOut1 = model->addOperand(&type53);
  auto roiOut1 = model->addOperand(&type36);
  auto batchSplit1 = model->addOperand(&type34);
  // Phase 2, operations
  static float param6_init[] = {10.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {10.0f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t param8_init[] = {32};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {16};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static float param10_init[] = {0.2f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {1.0f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GENERATE_PROPOSALS, {scores1, bboxDeltas1, anchors1, imageInfo1, param6, param7, param8, param9, param10, param11, layout}, {scoresOut1, roiOut1, batchSplit1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores1, bboxDeltas1, anchors1, imageInfo1},
    {scoresOut1, roiOut1, batchSplit1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type25(Type::FLOAT16, {});
  OperandType type26(Type::TENSOR_FLOAT16, {4, 4});
  OperandType type34(Type::TENSOR_INT32, {0});
  OperandType type37(Type::TENSOR_FLOAT16, {0});
  OperandType type38(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type46(Type::TENSOR_FLOAT16, {2, 2});
  OperandType type48(Type::TENSOR_FLOAT16, {2, 4, 4, 4});
  OperandType type52(Type::TENSOR_FLOAT16, {2, 16, 4, 4});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type48);
  auto bboxDeltas1 = model->addOperand(&type52);
  auto anchors1 = model->addOperand(&type26);
  auto imageInfo1 = model->addOperand(&type46);
  auto param6 = model->addOperand(&type25);
  auto param7 = model->addOperand(&type25);
  auto param8 = model->addOperand(&type9);
  auto param9 = model->addOperand(&type9);
  auto param10 = model->addOperand(&type25);
  auto param11 = model->addOperand(&type25);
  auto layout = model->addOperand(&type0);
  auto scoresOut1 = model->addOperand(&type37);
  auto roiOut1 = model->addOperand(&type38);
  auto batchSplit1 = model->addOperand(&type34);
  // Phase 2, operations
  static _Float16 param6_init[] = {10.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {10.0f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t param8_init[] = {32};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {16};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static _Float16 param10_init[] = {0.20000000298023224f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {1.0f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GENERATE_PROPOSALS, {scores1, bboxDeltas1, anchors1, imageInfo1, param6, param7, param8, param9, param10, param11, layout}, {scoresOut1, roiOut1, batchSplit1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores1, bboxDeltas1, anchors1, imageInfo1},
    {scoresOut1, roiOut1, batchSplit1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

