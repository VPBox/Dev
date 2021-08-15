// clang-format off
// Generated file (from: transpose_v1_2.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2});
  OperandType type1(Type::TENSOR_INT32, {0});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto perms = model->addOperand(&type1);
  auto output = model->addOperand(&type0);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_TRANSPOSE, {input, perms}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, perms},
    {output});
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2});
  OperandType type1(Type::TENSOR_INT32, {0});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto perms = model->addOperand(&type1);
  auto output = model->addOperand(&type0);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_TRANSPOSE, {input, perms}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, perms},
    {output});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant8(Model *model) {
  OperandType type1(Type::TENSOR_INT32, {0});
  OperandType type14(Type::TENSOR_QUANT8_ASYMM, {2, 2}, 0.5f, 0);
  // Phase 1, operands
  auto input = model->addOperand(&type14);
  auto perms = model->addOperand(&type1);
  auto output = model->addOperand(&type14);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_TRANSPOSE, {input, perms}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, perms},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2});
  OperandType type1(Type::TENSOR_INT32, {0});
  OperandType type15(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto perms = model->addOperand(&type1);
  auto output = model->addOperand(&type15);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_TRANSPOSE, {input, perms}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, perms},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2});
  OperandType type1(Type::TENSOR_INT32, {0});
  OperandType type15(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto perms = model->addOperand(&type1);
  auto output = model->addOperand(&type15);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_TRANSPOSE, {input, perms}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, perms},
    {output});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_quant8(Model *model) {
  OperandType type1(Type::TENSOR_INT32, {0});
  OperandType type14(Type::TENSOR_QUANT8_ASYMM, {2, 2}, 0.5f, 0);
  OperandType type16(Type::TENSOR_QUANT8_ASYMM, {0, 0}, 0.5f, 0);
  // Phase 1, operands
  auto input = model->addOperand(&type14);
  auto perms = model->addOperand(&type1);
  auto output = model->addOperand(&type16);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_TRANSPOSE, {input, perms}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, perms},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized(Model *model) {
  OperandType type1(Type::TENSOR_INT32, {0});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type12(Type::TENSOR_FLOAT32, {0, 1, 2, 2});
  OperandType type13(Type::TENSOR_INT32, {4});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type4(Type::TENSOR_FLOAT32, {0});
  OperandType type5(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type6(Type::TENSOR_INT32, {1});
  OperandType type7(Type::FLOAT32, {});
  OperandType type8(Type::INT32, {});
  OperandType type9(Type::BOOL, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type2);
  auto roi = model->addOperand(&type3);
  auto param = model->addOperand(&type6);
  auto param1 = model->addOperand(&type7);
  auto param2 = model->addOperand(&type8);
  auto param3 = model->addOperand(&type8);
  auto param4 = model->addOperand(&type7);
  auto param5 = model->addOperand(&type7);
  auto param6 = model->addOperand(&type7);
  auto scoresOut = model->addOperand(&type4);
  auto roiOut = model->addOperand(&type5);
  auto classesOut = model->addOperand(&type1);
  auto batchSplitOut = model->addOperand(&type1);
  auto in = model->addOperand(&type10);
  auto param7 = model->addOperand(&type8);
  auto param8 = model->addOperand(&type8);
  auto param9 = model->addOperand(&type7);
  auto param10 = model->addOperand(&type7);
  auto param11 = model->addOperand(&type8);
  auto param12 = model->addOperand(&type8);
  auto layout = model->addOperand(&type9);
  auto featureMap = model->addOperand(&type11);
  auto param13 = model->addOperand(&type13);
  auto out = model->addOperand(&type12);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.3f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static int32_t param2_init[] = {-1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static float param4_init[] = {0.4f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {1.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {0.3f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static int32_t param7_init[] = {2};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {2.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {2.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static int32_t param11_init[] = {4};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {4};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param13_init[] = {0, 3, 1, 2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 4);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param, param1, param2, param3, param4, param5, param6}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param7, param8, param9, param10, param11, param12, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_TRANSPOSE, {featureMap, param13}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_relaxed(Model *model) {
  OperandType type1(Type::TENSOR_INT32, {0});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type12(Type::TENSOR_FLOAT32, {0, 1, 2, 2});
  OperandType type13(Type::TENSOR_INT32, {4});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type4(Type::TENSOR_FLOAT32, {0});
  OperandType type5(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type6(Type::TENSOR_INT32, {1});
  OperandType type7(Type::FLOAT32, {});
  OperandType type8(Type::INT32, {});
  OperandType type9(Type::BOOL, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type2);
  auto roi = model->addOperand(&type3);
  auto param = model->addOperand(&type6);
  auto param1 = model->addOperand(&type7);
  auto param2 = model->addOperand(&type8);
  auto param3 = model->addOperand(&type8);
  auto param4 = model->addOperand(&type7);
  auto param5 = model->addOperand(&type7);
  auto param6 = model->addOperand(&type7);
  auto scoresOut = model->addOperand(&type4);
  auto roiOut = model->addOperand(&type5);
  auto classesOut = model->addOperand(&type1);
  auto batchSplitOut = model->addOperand(&type1);
  auto in = model->addOperand(&type10);
  auto param7 = model->addOperand(&type8);
  auto param8 = model->addOperand(&type8);
  auto param9 = model->addOperand(&type7);
  auto param10 = model->addOperand(&type7);
  auto param11 = model->addOperand(&type8);
  auto param12 = model->addOperand(&type8);
  auto layout = model->addOperand(&type9);
  auto featureMap = model->addOperand(&type11);
  auto param13 = model->addOperand(&type13);
  auto out = model->addOperand(&type12);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.3f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static int32_t param2_init[] = {-1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static float param4_init[] = {0.4f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {1.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {0.3f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static int32_t param7_init[] = {2};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {2.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {2.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static int32_t param11_init[] = {4};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {4};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param13_init[] = {0, 3, 1, 2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 4);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param, param1, param2, param3, param4, param5, param6}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param7, param8, param9, param10, param11, param12, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_TRANSPOSE, {featureMap, param13}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_quant8(Model *model) {
  OperandType type1(Type::TENSOR_INT32, {0});
  OperandType type13(Type::TENSOR_INT32, {4});
  OperandType type17(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 1}, 0.1f, 128);
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.1f, 128);
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {0, 1, 2, 2}, 0.1f, 128);
  OperandType type20(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type21(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type22(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  OperandType type6(Type::TENSOR_INT32, {1});
  OperandType type7(Type::FLOAT32, {});
  OperandType type8(Type::INT32, {});
  OperandType type9(Type::BOOL, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type22);
  auto roi = model->addOperand(&type20);
  auto param = model->addOperand(&type6);
  auto param1 = model->addOperand(&type7);
  auto param2 = model->addOperand(&type8);
  auto param3 = model->addOperand(&type8);
  auto param4 = model->addOperand(&type7);
  auto param5 = model->addOperand(&type7);
  auto param6 = model->addOperand(&type7);
  auto scoresOut = model->addOperand(&type23);
  auto roiOut = model->addOperand(&type21);
  auto classesOut = model->addOperand(&type1);
  auto batchSplitOut = model->addOperand(&type1);
  auto in = model->addOperand(&type18);
  auto param7 = model->addOperand(&type8);
  auto param8 = model->addOperand(&type8);
  auto param9 = model->addOperand(&type7);
  auto param10 = model->addOperand(&type7);
  auto param11 = model->addOperand(&type8);
  auto param12 = model->addOperand(&type8);
  auto layout = model->addOperand(&type9);
  auto featureMap = model->addOperand(&type17);
  auto param13 = model->addOperand(&type13);
  auto out = model->addOperand(&type19);
  // Phase 2, operations
  static uint8_t scores_init[] = {137, 129};
  model->setOperandValue(scores, scores_init, sizeof(uint8_t) * 2);
  static uint16_t roi_init[] = {8, 8, 80, 80, 0, 0, 80, 80};
  model->setOperandValue(roi, roi_init, sizeof(uint16_t) * 8);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.3f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static int32_t param2_init[] = {-1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static float param4_init[] = {0.4f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {1.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {0.3f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static int32_t param7_init[] = {2};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {2.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {2.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static int32_t param11_init[] = {4};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {4};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param13_init[] = {0, 3, 1, 2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 4);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param, param1, param2, param3, param4, param5, param6}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param7, param8, param9, param10, param11, param12, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_TRANSPOSE, {featureMap, param13}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_float16(Model *model) {
  OperandType type1(Type::TENSOR_INT32, {0});
  OperandType type13(Type::TENSOR_INT32, {4});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 2, 2, 1});
  OperandType type25(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 1, 2, 2});
  OperandType type27(Type::FLOAT16, {});
  OperandType type28(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type29(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type30(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type31(Type::TENSOR_FLOAT16, {0});
  OperandType type6(Type::TENSOR_INT32, {1});
  OperandType type8(Type::INT32, {});
  OperandType type9(Type::BOOL, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type30);
  auto roi = model->addOperand(&type28);
  auto param = model->addOperand(&type6);
  auto param1 = model->addOperand(&type27);
  auto param2 = model->addOperand(&type8);
  auto param3 = model->addOperand(&type8);
  auto param4 = model->addOperand(&type27);
  auto param5 = model->addOperand(&type27);
  auto param6 = model->addOperand(&type27);
  auto scoresOut = model->addOperand(&type31);
  auto roiOut = model->addOperand(&type29);
  auto classesOut = model->addOperand(&type1);
  auto batchSplitOut = model->addOperand(&type1);
  auto in = model->addOperand(&type25);
  auto param7 = model->addOperand(&type8);
  auto param8 = model->addOperand(&type8);
  auto param9 = model->addOperand(&type27);
  auto param10 = model->addOperand(&type27);
  auto param11 = model->addOperand(&type8);
  auto param12 = model->addOperand(&type8);
  auto layout = model->addOperand(&type9);
  auto featureMap = model->addOperand(&type24);
  auto param13 = model->addOperand(&type13);
  auto out = model->addOperand(&type26);
  // Phase 2, operations
  static _Float16 scores_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores, scores_init, sizeof(_Float16) * 2);
  static _Float16 roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(_Float16) * 8);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.30000001192092896f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static int32_t param2_init[] = {-1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static _Float16 param4_init[] = {0.4000000059604645f};
  model->setOperandValue(param4, param4_init, sizeof(_Float16) * 1);
  static _Float16 param5_init[] = {1.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {0.30000001192092896f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static int32_t param7_init[] = {2};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {2.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {2.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static int32_t param11_init[] = {4};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {4};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param13_init[] = {0, 3, 1, 2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 4);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param, param1, param2, param3, param4, param5, param6}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param7, param8, param9, param10, param11, param12, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_TRANSPOSE, {featureMap, param13}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape(Model *model) {
  OperandType type1(Type::TENSOR_INT32, {0});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type13(Type::TENSOR_INT32, {4});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type32(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::TENSOR_FLOAT32, {0});
  OperandType type5(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type6(Type::TENSOR_INT32, {1});
  OperandType type7(Type::FLOAT32, {});
  OperandType type8(Type::INT32, {});
  OperandType type9(Type::BOOL, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type2);
  auto roi = model->addOperand(&type3);
  auto param = model->addOperand(&type6);
  auto param1 = model->addOperand(&type7);
  auto param2 = model->addOperand(&type8);
  auto param3 = model->addOperand(&type8);
  auto param4 = model->addOperand(&type7);
  auto param5 = model->addOperand(&type7);
  auto param6 = model->addOperand(&type7);
  auto scoresOut = model->addOperand(&type4);
  auto roiOut = model->addOperand(&type5);
  auto classesOut = model->addOperand(&type1);
  auto batchSplitOut = model->addOperand(&type1);
  auto in = model->addOperand(&type10);
  auto param7 = model->addOperand(&type8);
  auto param8 = model->addOperand(&type8);
  auto param9 = model->addOperand(&type7);
  auto param10 = model->addOperand(&type7);
  auto param11 = model->addOperand(&type8);
  auto param12 = model->addOperand(&type8);
  auto layout = model->addOperand(&type9);
  auto featureMap = model->addOperand(&type11);
  auto param13 = model->addOperand(&type13);
  auto out = model->addOperand(&type32);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.3f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static int32_t param2_init[] = {-1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static float param4_init[] = {0.4f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {1.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {0.3f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static int32_t param7_init[] = {2};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {2.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {2.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static int32_t param11_init[] = {4};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {4};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param13_init[] = {0, 3, 1, 2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 4);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param, param1, param2, param3, param4, param5, param6}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param7, param8, param9, param10, param11, param12, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_TRANSPOSE, {featureMap, param13}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_relaxed(Model *model) {
  OperandType type1(Type::TENSOR_INT32, {0});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type13(Type::TENSOR_INT32, {4});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type32(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::TENSOR_FLOAT32, {0});
  OperandType type5(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type6(Type::TENSOR_INT32, {1});
  OperandType type7(Type::FLOAT32, {});
  OperandType type8(Type::INT32, {});
  OperandType type9(Type::BOOL, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type2);
  auto roi = model->addOperand(&type3);
  auto param = model->addOperand(&type6);
  auto param1 = model->addOperand(&type7);
  auto param2 = model->addOperand(&type8);
  auto param3 = model->addOperand(&type8);
  auto param4 = model->addOperand(&type7);
  auto param5 = model->addOperand(&type7);
  auto param6 = model->addOperand(&type7);
  auto scoresOut = model->addOperand(&type4);
  auto roiOut = model->addOperand(&type5);
  auto classesOut = model->addOperand(&type1);
  auto batchSplitOut = model->addOperand(&type1);
  auto in = model->addOperand(&type10);
  auto param7 = model->addOperand(&type8);
  auto param8 = model->addOperand(&type8);
  auto param9 = model->addOperand(&type7);
  auto param10 = model->addOperand(&type7);
  auto param11 = model->addOperand(&type8);
  auto param12 = model->addOperand(&type8);
  auto layout = model->addOperand(&type9);
  auto featureMap = model->addOperand(&type11);
  auto param13 = model->addOperand(&type13);
  auto out = model->addOperand(&type32);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.3f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static int32_t param2_init[] = {-1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static float param4_init[] = {0.4f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {1.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {0.3f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static int32_t param7_init[] = {2};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {2.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {2.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static int32_t param11_init[] = {4};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {4};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param13_init[] = {0, 3, 1, 2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 4);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param, param1, param2, param3, param4, param5, param6}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param7, param8, param9, param10, param11, param12, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_TRANSPOSE, {featureMap, param13}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_quant8(Model *model) {
  OperandType type1(Type::TENSOR_INT32, {0});
  OperandType type13(Type::TENSOR_INT32, {4});
  OperandType type17(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 1}, 0.1f, 128);
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.1f, 128);
  OperandType type20(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type21(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type22(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  OperandType type33(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 128);
  OperandType type6(Type::TENSOR_INT32, {1});
  OperandType type7(Type::FLOAT32, {});
  OperandType type8(Type::INT32, {});
  OperandType type9(Type::BOOL, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type22);
  auto roi = model->addOperand(&type20);
  auto param = model->addOperand(&type6);
  auto param1 = model->addOperand(&type7);
  auto param2 = model->addOperand(&type8);
  auto param3 = model->addOperand(&type8);
  auto param4 = model->addOperand(&type7);
  auto param5 = model->addOperand(&type7);
  auto param6 = model->addOperand(&type7);
  auto scoresOut = model->addOperand(&type23);
  auto roiOut = model->addOperand(&type21);
  auto classesOut = model->addOperand(&type1);
  auto batchSplitOut = model->addOperand(&type1);
  auto in = model->addOperand(&type18);
  auto param7 = model->addOperand(&type8);
  auto param8 = model->addOperand(&type8);
  auto param9 = model->addOperand(&type7);
  auto param10 = model->addOperand(&type7);
  auto param11 = model->addOperand(&type8);
  auto param12 = model->addOperand(&type8);
  auto layout = model->addOperand(&type9);
  auto featureMap = model->addOperand(&type17);
  auto param13 = model->addOperand(&type13);
  auto out = model->addOperand(&type33);
  // Phase 2, operations
  static uint8_t scores_init[] = {137, 129};
  model->setOperandValue(scores, scores_init, sizeof(uint8_t) * 2);
  static uint16_t roi_init[] = {8, 8, 80, 80, 0, 0, 80, 80};
  model->setOperandValue(roi, roi_init, sizeof(uint16_t) * 8);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.3f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static int32_t param2_init[] = {-1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static float param4_init[] = {0.4f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {1.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {0.3f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static int32_t param7_init[] = {2};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {2.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {2.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static int32_t param11_init[] = {4};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {4};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param13_init[] = {0, 3, 1, 2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 4);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param, param1, param2, param3, param4, param5, param6}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param7, param8, param9, param10, param11, param12, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_TRANSPOSE, {featureMap, param13}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_float16(Model *model) {
  OperandType type1(Type::TENSOR_INT32, {0});
  OperandType type13(Type::TENSOR_INT32, {4});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 2, 2, 1});
  OperandType type25(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type27(Type::FLOAT16, {});
  OperandType type28(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type29(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type30(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type34(Type::TENSOR_FLOAT16, {0});
  OperandType type35(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type6(Type::TENSOR_INT32, {1});
  OperandType type8(Type::INT32, {});
  OperandType type9(Type::BOOL, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type30);
  auto roi = model->addOperand(&type28);
  auto param = model->addOperand(&type6);
  auto param1 = model->addOperand(&type27);
  auto param2 = model->addOperand(&type8);
  auto param3 = model->addOperand(&type8);
  auto param4 = model->addOperand(&type27);
  auto param5 = model->addOperand(&type27);
  auto param6 = model->addOperand(&type27);
  auto scoresOut = model->addOperand(&type34);
  auto roiOut = model->addOperand(&type29);
  auto classesOut = model->addOperand(&type1);
  auto batchSplitOut = model->addOperand(&type1);
  auto in = model->addOperand(&type25);
  auto param7 = model->addOperand(&type8);
  auto param8 = model->addOperand(&type8);
  auto param9 = model->addOperand(&type27);
  auto param10 = model->addOperand(&type27);
  auto param11 = model->addOperand(&type8);
  auto param12 = model->addOperand(&type8);
  auto layout = model->addOperand(&type9);
  auto featureMap = model->addOperand(&type24);
  auto param13 = model->addOperand(&type13);
  auto out = model->addOperand(&type35);
  // Phase 2, operations
  static _Float16 scores_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores, scores_init, sizeof(_Float16) * 2);
  static _Float16 roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(_Float16) * 8);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.30000001192092896f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static int32_t param2_init[] = {-1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static _Float16 param4_init[] = {0.4000000059604645f};
  model->setOperandValue(param4, param4_init, sizeof(_Float16) * 1);
  static _Float16 param5_init[] = {1.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {0.30000001192092896f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static int32_t param7_init[] = {2};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {2.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {2.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static int32_t param11_init[] = {4};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {4};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param13_init[] = {0, 3, 1, 2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 4);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param, param1, param2, param3, param4, param5, param6}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param7, param8, param9, param10, param11, param12, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_TRANSPOSE, {featureMap, param13}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

