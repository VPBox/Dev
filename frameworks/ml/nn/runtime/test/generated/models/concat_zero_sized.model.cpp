// clang-format off
// Generated file (from: concat_zero_sized.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type10(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 2, 2, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {0});
  OperandType type3(Type::TENSOR_INT32, {0});
  OperandType type4(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type5(Type::TENSOR_INT32, {1});
  OperandType type6(Type::FLOAT32, {});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  // Phase 1, operands
  auto scores = model->addOperand(&type0);
  auto roi = model->addOperand(&type1);
  auto param = model->addOperand(&type5);
  auto param1 = model->addOperand(&type6);
  auto param2 = model->addOperand(&type7);
  auto param3 = model->addOperand(&type7);
  auto param4 = model->addOperand(&type6);
  auto param5 = model->addOperand(&type6);
  auto param6 = model->addOperand(&type6);
  auto scoresOut = model->addOperand(&type2);
  auto roiOut = model->addOperand(&type4);
  auto classesOut = model->addOperand(&type3);
  auto batchSplitOut = model->addOperand(&type3);
  auto in = model->addOperand(&type9);
  auto param7 = model->addOperand(&type7);
  auto param8 = model->addOperand(&type7);
  auto param9 = model->addOperand(&type6);
  auto param10 = model->addOperand(&type6);
  auto param11 = model->addOperand(&type7);
  auto param12 = model->addOperand(&type7);
  auto layout = model->addOperand(&type8);
  auto featureMap = model->addOperand(&type10);
  auto param13 = model->addOperand(&type7);
  auto out = model->addOperand(&type11);
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
  static int32_t param13_init[] = {3};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param, param1, param2, param3, param4, param5, param6}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param7, param8, param9, param10, param11, param12, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_CONCATENATION, {featureMap, featureMap, param13}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type10(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 2, 2, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {0});
  OperandType type3(Type::TENSOR_INT32, {0});
  OperandType type4(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type5(Type::TENSOR_INT32, {1});
  OperandType type6(Type::FLOAT32, {});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  // Phase 1, operands
  auto scores = model->addOperand(&type0);
  auto roi = model->addOperand(&type1);
  auto param = model->addOperand(&type5);
  auto param1 = model->addOperand(&type6);
  auto param2 = model->addOperand(&type7);
  auto param3 = model->addOperand(&type7);
  auto param4 = model->addOperand(&type6);
  auto param5 = model->addOperand(&type6);
  auto param6 = model->addOperand(&type6);
  auto scoresOut = model->addOperand(&type2);
  auto roiOut = model->addOperand(&type4);
  auto classesOut = model->addOperand(&type3);
  auto batchSplitOut = model->addOperand(&type3);
  auto in = model->addOperand(&type9);
  auto param7 = model->addOperand(&type7);
  auto param8 = model->addOperand(&type7);
  auto param9 = model->addOperand(&type6);
  auto param10 = model->addOperand(&type6);
  auto param11 = model->addOperand(&type7);
  auto param12 = model->addOperand(&type7);
  auto layout = model->addOperand(&type8);
  auto featureMap = model->addOperand(&type10);
  auto param13 = model->addOperand(&type7);
  auto out = model->addOperand(&type11);
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
  static int32_t param13_init[] = {3};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param, param1, param2, param3, param4, param5, param6}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param7, param8, param9, param10, param11, param12, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_CONCATENATION, {featureMap, featureMap, param13}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant8(Model *model) {
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 1}, 0.1f, 128);
  OperandType type14(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.1f, 128);
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 2}, 0.1f, 128);
  OperandType type16(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type17(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  OperandType type3(Type::TENSOR_INT32, {0});
  OperandType type5(Type::TENSOR_INT32, {1});
  OperandType type6(Type::FLOAT32, {});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type18);
  auto roi = model->addOperand(&type16);
  auto param = model->addOperand(&type5);
  auto param1 = model->addOperand(&type6);
  auto param2 = model->addOperand(&type7);
  auto param3 = model->addOperand(&type7);
  auto param4 = model->addOperand(&type6);
  auto param5 = model->addOperand(&type6);
  auto param6 = model->addOperand(&type6);
  auto scoresOut = model->addOperand(&type19);
  auto roiOut = model->addOperand(&type17);
  auto classesOut = model->addOperand(&type3);
  auto batchSplitOut = model->addOperand(&type3);
  auto in = model->addOperand(&type14);
  auto param7 = model->addOperand(&type7);
  auto param8 = model->addOperand(&type7);
  auto param9 = model->addOperand(&type6);
  auto param10 = model->addOperand(&type6);
  auto param11 = model->addOperand(&type7);
  auto param12 = model->addOperand(&type7);
  auto layout = model->addOperand(&type8);
  auto featureMap = model->addOperand(&type13);
  auto param13 = model->addOperand(&type7);
  auto out = model->addOperand(&type15);
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
  static int32_t param13_init[] = {3};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param, param1, param2, param3, param4, param5, param6}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param7, param8, param9, param10, param11, param12, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_CONCATENATION, {featureMap, featureMap, param13}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16(Model *model) {
  OperandType type20(Type::TENSOR_FLOAT16, {0, 2, 2, 1});
  OperandType type21(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type22(Type::TENSOR_FLOAT16, {0, 2, 2, 2});
  OperandType type23(Type::FLOAT16, {});
  OperandType type24(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type25(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type26(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type27(Type::TENSOR_FLOAT16, {0});
  OperandType type3(Type::TENSOR_INT32, {0});
  OperandType type5(Type::TENSOR_INT32, {1});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type26);
  auto roi = model->addOperand(&type24);
  auto param = model->addOperand(&type5);
  auto param1 = model->addOperand(&type23);
  auto param2 = model->addOperand(&type7);
  auto param3 = model->addOperand(&type7);
  auto param4 = model->addOperand(&type23);
  auto param5 = model->addOperand(&type23);
  auto param6 = model->addOperand(&type23);
  auto scoresOut = model->addOperand(&type27);
  auto roiOut = model->addOperand(&type25);
  auto classesOut = model->addOperand(&type3);
  auto batchSplitOut = model->addOperand(&type3);
  auto in = model->addOperand(&type21);
  auto param7 = model->addOperand(&type7);
  auto param8 = model->addOperand(&type7);
  auto param9 = model->addOperand(&type23);
  auto param10 = model->addOperand(&type23);
  auto param11 = model->addOperand(&type7);
  auto param12 = model->addOperand(&type7);
  auto layout = model->addOperand(&type8);
  auto featureMap = model->addOperand(&type20);
  auto param13 = model->addOperand(&type7);
  auto out = model->addOperand(&type22);
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
  static int32_t param13_init[] = {3};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param, param1, param2, param3, param4, param5, param6}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param7, param8, param9, param10, param11, param12, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_CONCATENATION, {featureMap, featureMap, param13}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type10(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {0});
  OperandType type28(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_INT32, {0});
  OperandType type4(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type5(Type::TENSOR_INT32, {1});
  OperandType type6(Type::FLOAT32, {});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  // Phase 1, operands
  auto scores = model->addOperand(&type0);
  auto roi = model->addOperand(&type1);
  auto param = model->addOperand(&type5);
  auto param1 = model->addOperand(&type6);
  auto param2 = model->addOperand(&type7);
  auto param3 = model->addOperand(&type7);
  auto param4 = model->addOperand(&type6);
  auto param5 = model->addOperand(&type6);
  auto param6 = model->addOperand(&type6);
  auto scoresOut = model->addOperand(&type2);
  auto roiOut = model->addOperand(&type4);
  auto classesOut = model->addOperand(&type3);
  auto batchSplitOut = model->addOperand(&type3);
  auto in = model->addOperand(&type9);
  auto param7 = model->addOperand(&type7);
  auto param8 = model->addOperand(&type7);
  auto param9 = model->addOperand(&type6);
  auto param10 = model->addOperand(&type6);
  auto param11 = model->addOperand(&type7);
  auto param12 = model->addOperand(&type7);
  auto layout = model->addOperand(&type8);
  auto featureMap = model->addOperand(&type10);
  auto param13 = model->addOperand(&type7);
  auto out = model->addOperand(&type28);
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
  static int32_t param13_init[] = {3};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param, param1, param2, param3, param4, param5, param6}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param7, param8, param9, param10, param11, param12, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_CONCATENATION, {featureMap, featureMap, param13}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type10(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {0});
  OperandType type28(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_INT32, {0});
  OperandType type4(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type5(Type::TENSOR_INT32, {1});
  OperandType type6(Type::FLOAT32, {});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  // Phase 1, operands
  auto scores = model->addOperand(&type0);
  auto roi = model->addOperand(&type1);
  auto param = model->addOperand(&type5);
  auto param1 = model->addOperand(&type6);
  auto param2 = model->addOperand(&type7);
  auto param3 = model->addOperand(&type7);
  auto param4 = model->addOperand(&type6);
  auto param5 = model->addOperand(&type6);
  auto param6 = model->addOperand(&type6);
  auto scoresOut = model->addOperand(&type2);
  auto roiOut = model->addOperand(&type4);
  auto classesOut = model->addOperand(&type3);
  auto batchSplitOut = model->addOperand(&type3);
  auto in = model->addOperand(&type9);
  auto param7 = model->addOperand(&type7);
  auto param8 = model->addOperand(&type7);
  auto param9 = model->addOperand(&type6);
  auto param10 = model->addOperand(&type6);
  auto param11 = model->addOperand(&type7);
  auto param12 = model->addOperand(&type7);
  auto layout = model->addOperand(&type8);
  auto featureMap = model->addOperand(&type10);
  auto param13 = model->addOperand(&type7);
  auto out = model->addOperand(&type28);
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
  static int32_t param13_init[] = {3};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param, param1, param2, param3, param4, param5, param6}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param7, param8, param9, param10, param11, param12, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_CONCATENATION, {featureMap, featureMap, param13}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_quant8(Model *model) {
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 1}, 0.1f, 128);
  OperandType type14(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.1f, 128);
  OperandType type16(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type17(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  OperandType type29(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 128);
  OperandType type3(Type::TENSOR_INT32, {0});
  OperandType type5(Type::TENSOR_INT32, {1});
  OperandType type6(Type::FLOAT32, {});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type18);
  auto roi = model->addOperand(&type16);
  auto param = model->addOperand(&type5);
  auto param1 = model->addOperand(&type6);
  auto param2 = model->addOperand(&type7);
  auto param3 = model->addOperand(&type7);
  auto param4 = model->addOperand(&type6);
  auto param5 = model->addOperand(&type6);
  auto param6 = model->addOperand(&type6);
  auto scoresOut = model->addOperand(&type19);
  auto roiOut = model->addOperand(&type17);
  auto classesOut = model->addOperand(&type3);
  auto batchSplitOut = model->addOperand(&type3);
  auto in = model->addOperand(&type14);
  auto param7 = model->addOperand(&type7);
  auto param8 = model->addOperand(&type7);
  auto param9 = model->addOperand(&type6);
  auto param10 = model->addOperand(&type6);
  auto param11 = model->addOperand(&type7);
  auto param12 = model->addOperand(&type7);
  auto layout = model->addOperand(&type8);
  auto featureMap = model->addOperand(&type13);
  auto param13 = model->addOperand(&type7);
  auto out = model->addOperand(&type29);
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
  static int32_t param13_init[] = {3};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param, param1, param2, param3, param4, param5, param6}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param7, param8, param9, param10, param11, param12, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_CONCATENATION, {featureMap, featureMap, param13}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16(Model *model) {
  OperandType type20(Type::TENSOR_FLOAT16, {0, 2, 2, 1});
  OperandType type21(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type23(Type::FLOAT16, {});
  OperandType type24(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type25(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type26(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type3(Type::TENSOR_INT32, {0});
  OperandType type30(Type::TENSOR_FLOAT16, {0});
  OperandType type31(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::TENSOR_INT32, {1});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type26);
  auto roi = model->addOperand(&type24);
  auto param = model->addOperand(&type5);
  auto param1 = model->addOperand(&type23);
  auto param2 = model->addOperand(&type7);
  auto param3 = model->addOperand(&type7);
  auto param4 = model->addOperand(&type23);
  auto param5 = model->addOperand(&type23);
  auto param6 = model->addOperand(&type23);
  auto scoresOut = model->addOperand(&type30);
  auto roiOut = model->addOperand(&type25);
  auto classesOut = model->addOperand(&type3);
  auto batchSplitOut = model->addOperand(&type3);
  auto in = model->addOperand(&type21);
  auto param7 = model->addOperand(&type7);
  auto param8 = model->addOperand(&type7);
  auto param9 = model->addOperand(&type23);
  auto param10 = model->addOperand(&type23);
  auto param11 = model->addOperand(&type7);
  auto param12 = model->addOperand(&type7);
  auto layout = model->addOperand(&type8);
  auto featureMap = model->addOperand(&type20);
  auto param13 = model->addOperand(&type7);
  auto out = model->addOperand(&type31);
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
  static int32_t param13_init[] = {3};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param, param1, param2, param3, param4, param5, param6}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param7, param8, param9, param10, param11, param12, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_CONCATENATION, {featureMap, featureMap, param13}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type10(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type12(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {0});
  OperandType type3(Type::TENSOR_INT32, {0});
  OperandType type4(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type5(Type::TENSOR_INT32, {1});
  OperandType type6(Type::FLOAT32, {});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type0);
  auto roi1 = model->addOperand(&type1);
  auto param14 = model->addOperand(&type5);
  auto param15 = model->addOperand(&type6);
  auto param16 = model->addOperand(&type7);
  auto param17 = model->addOperand(&type7);
  auto param18 = model->addOperand(&type6);
  auto param19 = model->addOperand(&type6);
  auto param20 = model->addOperand(&type6);
  auto scoresOut1 = model->addOperand(&type2);
  auto roiOut1 = model->addOperand(&type4);
  auto classesOut1 = model->addOperand(&type3);
  auto batchSplitOut1 = model->addOperand(&type3);
  auto in1 = model->addOperand(&type9);
  auto param21 = model->addOperand(&type7);
  auto param22 = model->addOperand(&type7);
  auto param23 = model->addOperand(&type6);
  auto param24 = model->addOperand(&type6);
  auto param25 = model->addOperand(&type7);
  auto param26 = model->addOperand(&type7);
  auto layout1 = model->addOperand(&type8);
  auto featureMap1 = model->addOperand(&type10);
  auto in2 = model->addOperand(&type12);
  auto param27 = model->addOperand(&type7);
  auto out1 = model->addOperand(&type12);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static float param15_init[] = {0.3f};
  model->setOperandValue(param15, param15_init, sizeof(float) * 1);
  static int32_t param16_init[] = {-1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static float param18_init[] = {0.4f};
  model->setOperandValue(param18, param18_init, sizeof(float) * 1);
  static float param19_init[] = {1.0f};
  model->setOperandValue(param19, param19_init, sizeof(float) * 1);
  static float param20_init[] = {0.3f};
  model->setOperandValue(param20, param20_init, sizeof(float) * 1);
  static int32_t param21_init[] = {2};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static float param23_init[] = {2.0f};
  model->setOperandValue(param23, param23_init, sizeof(float) * 1);
  static float param24_init[] = {2.0f};
  model->setOperandValue(param24, param24_init, sizeof(float) * 1);
  static int32_t param25_init[] = {4};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {4};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static bool8 layout1_init[] = {false};
  model->setOperandValue(layout1, layout1_init, sizeof(bool8) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param14, param15, param16, param17, param18, param19, param20}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param21, param22, param23, param24, param25, param26, layout1}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_CONCATENATION, {featureMap1, in2, param27}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1, in2},
    {scoresOut1, classesOut1, out1});
  assert(model->isValid());
}

inline bool is_ignored_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type10(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type12(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {0});
  OperandType type3(Type::TENSOR_INT32, {0});
  OperandType type4(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type5(Type::TENSOR_INT32, {1});
  OperandType type6(Type::FLOAT32, {});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type0);
  auto roi1 = model->addOperand(&type1);
  auto param14 = model->addOperand(&type5);
  auto param15 = model->addOperand(&type6);
  auto param16 = model->addOperand(&type7);
  auto param17 = model->addOperand(&type7);
  auto param18 = model->addOperand(&type6);
  auto param19 = model->addOperand(&type6);
  auto param20 = model->addOperand(&type6);
  auto scoresOut1 = model->addOperand(&type2);
  auto roiOut1 = model->addOperand(&type4);
  auto classesOut1 = model->addOperand(&type3);
  auto batchSplitOut1 = model->addOperand(&type3);
  auto in1 = model->addOperand(&type9);
  auto param21 = model->addOperand(&type7);
  auto param22 = model->addOperand(&type7);
  auto param23 = model->addOperand(&type6);
  auto param24 = model->addOperand(&type6);
  auto param25 = model->addOperand(&type7);
  auto param26 = model->addOperand(&type7);
  auto layout1 = model->addOperand(&type8);
  auto featureMap1 = model->addOperand(&type10);
  auto in2 = model->addOperand(&type12);
  auto param27 = model->addOperand(&type7);
  auto out1 = model->addOperand(&type12);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static float param15_init[] = {0.3f};
  model->setOperandValue(param15, param15_init, sizeof(float) * 1);
  static int32_t param16_init[] = {-1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static float param18_init[] = {0.4f};
  model->setOperandValue(param18, param18_init, sizeof(float) * 1);
  static float param19_init[] = {1.0f};
  model->setOperandValue(param19, param19_init, sizeof(float) * 1);
  static float param20_init[] = {0.3f};
  model->setOperandValue(param20, param20_init, sizeof(float) * 1);
  static int32_t param21_init[] = {2};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static float param23_init[] = {2.0f};
  model->setOperandValue(param23, param23_init, sizeof(float) * 1);
  static float param24_init[] = {2.0f};
  model->setOperandValue(param24, param24_init, sizeof(float) * 1);
  static int32_t param25_init[] = {4};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {4};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static bool8 layout1_init[] = {false};
  model->setOperandValue(layout1, layout1_init, sizeof(bool8) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param14, param15, param16, param17, param18, param19, param20}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param21, param22, param23, param24, param25, param26, layout1}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_CONCATENATION, {featureMap1, in2, param27}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1, in2},
    {scoresOut1, classesOut1, out1});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant8_2(Model *model) {
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 1}, 0.1f, 128);
  OperandType type14(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.1f, 128);
  OperandType type16(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type17(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  OperandType type3(Type::TENSOR_INT32, {0});
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.2f, 128);
  OperandType type33(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.1f, 128);
  OperandType type5(Type::TENSOR_INT32, {1});
  OperandType type6(Type::FLOAT32, {});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type18);
  auto roi1 = model->addOperand(&type16);
  auto param14 = model->addOperand(&type5);
  auto param15 = model->addOperand(&type6);
  auto param16 = model->addOperand(&type7);
  auto param17 = model->addOperand(&type7);
  auto param18 = model->addOperand(&type6);
  auto param19 = model->addOperand(&type6);
  auto param20 = model->addOperand(&type6);
  auto scoresOut1 = model->addOperand(&type19);
  auto roiOut1 = model->addOperand(&type17);
  auto classesOut1 = model->addOperand(&type3);
  auto batchSplitOut1 = model->addOperand(&type3);
  auto in1 = model->addOperand(&type14);
  auto param21 = model->addOperand(&type7);
  auto param22 = model->addOperand(&type7);
  auto param23 = model->addOperand(&type6);
  auto param24 = model->addOperand(&type6);
  auto param25 = model->addOperand(&type7);
  auto param26 = model->addOperand(&type7);
  auto layout1 = model->addOperand(&type8);
  auto featureMap1 = model->addOperand(&type13);
  auto in2 = model->addOperand(&type32);
  auto param27 = model->addOperand(&type7);
  auto out1 = model->addOperand(&type33);
  // Phase 2, operations
  static uint8_t scores1_init[] = {137, 129};
  model->setOperandValue(scores1, scores1_init, sizeof(uint8_t) * 2);
  static uint16_t roi1_init[] = {8, 8, 80, 80, 0, 0, 80, 80};
  model->setOperandValue(roi1, roi1_init, sizeof(uint16_t) * 8);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static float param15_init[] = {0.3f};
  model->setOperandValue(param15, param15_init, sizeof(float) * 1);
  static int32_t param16_init[] = {-1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static float param18_init[] = {0.4f};
  model->setOperandValue(param18, param18_init, sizeof(float) * 1);
  static float param19_init[] = {1.0f};
  model->setOperandValue(param19, param19_init, sizeof(float) * 1);
  static float param20_init[] = {0.3f};
  model->setOperandValue(param20, param20_init, sizeof(float) * 1);
  static int32_t param21_init[] = {2};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static float param23_init[] = {2.0f};
  model->setOperandValue(param23, param23_init, sizeof(float) * 1);
  static float param24_init[] = {2.0f};
  model->setOperandValue(param24, param24_init, sizeof(float) * 1);
  static int32_t param25_init[] = {4};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {4};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static bool8 layout1_init[] = {false};
  model->setOperandValue(layout1, layout1_init, sizeof(bool8) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param14, param15, param16, param17, param18, param19, param20}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param21, param22, param23, param24, param25, param26, layout1}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_CONCATENATION, {featureMap1, in2, param27}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1, in2},
    {scoresOut1, classesOut1, out1});
  assert(model->isValid());
}

inline bool is_ignored_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_2(Model *model) {
  OperandType type20(Type::TENSOR_FLOAT16, {0, 2, 2, 1});
  OperandType type21(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type23(Type::FLOAT16, {});
  OperandType type24(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type25(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type26(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type27(Type::TENSOR_FLOAT16, {0});
  OperandType type3(Type::TENSOR_INT32, {0});
  OperandType type34(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type5(Type::TENSOR_INT32, {1});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type26);
  auto roi1 = model->addOperand(&type24);
  auto param14 = model->addOperand(&type5);
  auto param15 = model->addOperand(&type23);
  auto param16 = model->addOperand(&type7);
  auto param17 = model->addOperand(&type7);
  auto param18 = model->addOperand(&type23);
  auto param19 = model->addOperand(&type23);
  auto param20 = model->addOperand(&type23);
  auto scoresOut1 = model->addOperand(&type27);
  auto roiOut1 = model->addOperand(&type25);
  auto classesOut1 = model->addOperand(&type3);
  auto batchSplitOut1 = model->addOperand(&type3);
  auto in1 = model->addOperand(&type21);
  auto param21 = model->addOperand(&type7);
  auto param22 = model->addOperand(&type7);
  auto param23 = model->addOperand(&type23);
  auto param24 = model->addOperand(&type23);
  auto param25 = model->addOperand(&type7);
  auto param26 = model->addOperand(&type7);
  auto layout1 = model->addOperand(&type8);
  auto featureMap1 = model->addOperand(&type20);
  auto in2 = model->addOperand(&type34);
  auto param27 = model->addOperand(&type7);
  auto out1 = model->addOperand(&type34);
  // Phase 2, operations
  static _Float16 scores1_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores1, scores1_init, sizeof(_Float16) * 2);
  static _Float16 roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(_Float16) * 8);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static _Float16 param15_init[] = {0.30000001192092896f};
  model->setOperandValue(param15, param15_init, sizeof(_Float16) * 1);
  static int32_t param16_init[] = {-1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static _Float16 param18_init[] = {0.4000000059604645f};
  model->setOperandValue(param18, param18_init, sizeof(_Float16) * 1);
  static _Float16 param19_init[] = {1.0f};
  model->setOperandValue(param19, param19_init, sizeof(_Float16) * 1);
  static _Float16 param20_init[] = {0.30000001192092896f};
  model->setOperandValue(param20, param20_init, sizeof(_Float16) * 1);
  static int32_t param21_init[] = {2};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static _Float16 param23_init[] = {2.0f};
  model->setOperandValue(param23, param23_init, sizeof(_Float16) * 1);
  static _Float16 param24_init[] = {2.0f};
  model->setOperandValue(param24, param24_init, sizeof(_Float16) * 1);
  static int32_t param25_init[] = {4};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {4};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static bool8 layout1_init[] = {false};
  model->setOperandValue(layout1, layout1_init, sizeof(bool8) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param14, param15, param16, param17, param18, param19, param20}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param21, param22, param23, param24, param25, param26, layout1}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_CONCATENATION, {featureMap1, in2, param27}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1, in2},
    {scoresOut1, classesOut1, out1});
  assert(model->isValid());
}

inline bool is_ignored_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type10(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type12(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {0});
  OperandType type28(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_INT32, {0});
  OperandType type4(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type5(Type::TENSOR_INT32, {1});
  OperandType type6(Type::FLOAT32, {});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type0);
  auto roi1 = model->addOperand(&type1);
  auto param14 = model->addOperand(&type5);
  auto param15 = model->addOperand(&type6);
  auto param16 = model->addOperand(&type7);
  auto param17 = model->addOperand(&type7);
  auto param18 = model->addOperand(&type6);
  auto param19 = model->addOperand(&type6);
  auto param20 = model->addOperand(&type6);
  auto scoresOut1 = model->addOperand(&type2);
  auto roiOut1 = model->addOperand(&type4);
  auto classesOut1 = model->addOperand(&type3);
  auto batchSplitOut1 = model->addOperand(&type3);
  auto in1 = model->addOperand(&type9);
  auto param21 = model->addOperand(&type7);
  auto param22 = model->addOperand(&type7);
  auto param23 = model->addOperand(&type6);
  auto param24 = model->addOperand(&type6);
  auto param25 = model->addOperand(&type7);
  auto param26 = model->addOperand(&type7);
  auto layout1 = model->addOperand(&type8);
  auto featureMap1 = model->addOperand(&type10);
  auto in2 = model->addOperand(&type12);
  auto param27 = model->addOperand(&type7);
  auto out1 = model->addOperand(&type28);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static float param15_init[] = {0.3f};
  model->setOperandValue(param15, param15_init, sizeof(float) * 1);
  static int32_t param16_init[] = {-1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static float param18_init[] = {0.4f};
  model->setOperandValue(param18, param18_init, sizeof(float) * 1);
  static float param19_init[] = {1.0f};
  model->setOperandValue(param19, param19_init, sizeof(float) * 1);
  static float param20_init[] = {0.3f};
  model->setOperandValue(param20, param20_init, sizeof(float) * 1);
  static int32_t param21_init[] = {2};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static float param23_init[] = {2.0f};
  model->setOperandValue(param23, param23_init, sizeof(float) * 1);
  static float param24_init[] = {2.0f};
  model->setOperandValue(param24, param24_init, sizeof(float) * 1);
  static int32_t param25_init[] = {4};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {4};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static bool8 layout1_init[] = {false};
  model->setOperandValue(layout1, layout1_init, sizeof(bool8) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param14, param15, param16, param17, param18, param19, param20}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param21, param22, param23, param24, param25, param26, layout1}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_CONCATENATION, {featureMap1, in2, param27}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1, in2},
    {scoresOut1, classesOut1, out1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type10(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type12(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {0});
  OperandType type28(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_INT32, {0});
  OperandType type4(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type5(Type::TENSOR_INT32, {1});
  OperandType type6(Type::FLOAT32, {});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type0);
  auto roi1 = model->addOperand(&type1);
  auto param14 = model->addOperand(&type5);
  auto param15 = model->addOperand(&type6);
  auto param16 = model->addOperand(&type7);
  auto param17 = model->addOperand(&type7);
  auto param18 = model->addOperand(&type6);
  auto param19 = model->addOperand(&type6);
  auto param20 = model->addOperand(&type6);
  auto scoresOut1 = model->addOperand(&type2);
  auto roiOut1 = model->addOperand(&type4);
  auto classesOut1 = model->addOperand(&type3);
  auto batchSplitOut1 = model->addOperand(&type3);
  auto in1 = model->addOperand(&type9);
  auto param21 = model->addOperand(&type7);
  auto param22 = model->addOperand(&type7);
  auto param23 = model->addOperand(&type6);
  auto param24 = model->addOperand(&type6);
  auto param25 = model->addOperand(&type7);
  auto param26 = model->addOperand(&type7);
  auto layout1 = model->addOperand(&type8);
  auto featureMap1 = model->addOperand(&type10);
  auto in2 = model->addOperand(&type12);
  auto param27 = model->addOperand(&type7);
  auto out1 = model->addOperand(&type28);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static float param15_init[] = {0.3f};
  model->setOperandValue(param15, param15_init, sizeof(float) * 1);
  static int32_t param16_init[] = {-1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static float param18_init[] = {0.4f};
  model->setOperandValue(param18, param18_init, sizeof(float) * 1);
  static float param19_init[] = {1.0f};
  model->setOperandValue(param19, param19_init, sizeof(float) * 1);
  static float param20_init[] = {0.3f};
  model->setOperandValue(param20, param20_init, sizeof(float) * 1);
  static int32_t param21_init[] = {2};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static float param23_init[] = {2.0f};
  model->setOperandValue(param23, param23_init, sizeof(float) * 1);
  static float param24_init[] = {2.0f};
  model->setOperandValue(param24, param24_init, sizeof(float) * 1);
  static int32_t param25_init[] = {4};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {4};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static bool8 layout1_init[] = {false};
  model->setOperandValue(layout1, layout1_init, sizeof(bool8) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param14, param15, param16, param17, param18, param19, param20}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param21, param22, param23, param24, param25, param26, layout1}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_CONCATENATION, {featureMap1, in2, param27}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1, in2},
    {scoresOut1, classesOut1, out1});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_quant8_2(Model *model) {
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 1}, 0.1f, 128);
  OperandType type14(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.1f, 128);
  OperandType type16(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type17(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  OperandType type29(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 128);
  OperandType type3(Type::TENSOR_INT32, {0});
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.2f, 128);
  OperandType type5(Type::TENSOR_INT32, {1});
  OperandType type6(Type::FLOAT32, {});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type18);
  auto roi1 = model->addOperand(&type16);
  auto param14 = model->addOperand(&type5);
  auto param15 = model->addOperand(&type6);
  auto param16 = model->addOperand(&type7);
  auto param17 = model->addOperand(&type7);
  auto param18 = model->addOperand(&type6);
  auto param19 = model->addOperand(&type6);
  auto param20 = model->addOperand(&type6);
  auto scoresOut1 = model->addOperand(&type19);
  auto roiOut1 = model->addOperand(&type17);
  auto classesOut1 = model->addOperand(&type3);
  auto batchSplitOut1 = model->addOperand(&type3);
  auto in1 = model->addOperand(&type14);
  auto param21 = model->addOperand(&type7);
  auto param22 = model->addOperand(&type7);
  auto param23 = model->addOperand(&type6);
  auto param24 = model->addOperand(&type6);
  auto param25 = model->addOperand(&type7);
  auto param26 = model->addOperand(&type7);
  auto layout1 = model->addOperand(&type8);
  auto featureMap1 = model->addOperand(&type13);
  auto in2 = model->addOperand(&type32);
  auto param27 = model->addOperand(&type7);
  auto out1 = model->addOperand(&type29);
  // Phase 2, operations
  static uint8_t scores1_init[] = {137, 129};
  model->setOperandValue(scores1, scores1_init, sizeof(uint8_t) * 2);
  static uint16_t roi1_init[] = {8, 8, 80, 80, 0, 0, 80, 80};
  model->setOperandValue(roi1, roi1_init, sizeof(uint16_t) * 8);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static float param15_init[] = {0.3f};
  model->setOperandValue(param15, param15_init, sizeof(float) * 1);
  static int32_t param16_init[] = {-1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static float param18_init[] = {0.4f};
  model->setOperandValue(param18, param18_init, sizeof(float) * 1);
  static float param19_init[] = {1.0f};
  model->setOperandValue(param19, param19_init, sizeof(float) * 1);
  static float param20_init[] = {0.3f};
  model->setOperandValue(param20, param20_init, sizeof(float) * 1);
  static int32_t param21_init[] = {2};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static float param23_init[] = {2.0f};
  model->setOperandValue(param23, param23_init, sizeof(float) * 1);
  static float param24_init[] = {2.0f};
  model->setOperandValue(param24, param24_init, sizeof(float) * 1);
  static int32_t param25_init[] = {4};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {4};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static bool8 layout1_init[] = {false};
  model->setOperandValue(layout1, layout1_init, sizeof(bool8) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param14, param15, param16, param17, param18, param19, param20}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param21, param22, param23, param24, param25, param26, layout1}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_CONCATENATION, {featureMap1, in2, param27}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1, in2},
    {scoresOut1, classesOut1, out1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_2(Model *model) {
  OperandType type20(Type::TENSOR_FLOAT16, {0, 2, 2, 1});
  OperandType type21(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type23(Type::FLOAT16, {});
  OperandType type24(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type25(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type26(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type3(Type::TENSOR_INT32, {0});
  OperandType type30(Type::TENSOR_FLOAT16, {0});
  OperandType type31(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type34(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type5(Type::TENSOR_INT32, {1});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type26);
  auto roi1 = model->addOperand(&type24);
  auto param14 = model->addOperand(&type5);
  auto param15 = model->addOperand(&type23);
  auto param16 = model->addOperand(&type7);
  auto param17 = model->addOperand(&type7);
  auto param18 = model->addOperand(&type23);
  auto param19 = model->addOperand(&type23);
  auto param20 = model->addOperand(&type23);
  auto scoresOut1 = model->addOperand(&type30);
  auto roiOut1 = model->addOperand(&type25);
  auto classesOut1 = model->addOperand(&type3);
  auto batchSplitOut1 = model->addOperand(&type3);
  auto in1 = model->addOperand(&type21);
  auto param21 = model->addOperand(&type7);
  auto param22 = model->addOperand(&type7);
  auto param23 = model->addOperand(&type23);
  auto param24 = model->addOperand(&type23);
  auto param25 = model->addOperand(&type7);
  auto param26 = model->addOperand(&type7);
  auto layout1 = model->addOperand(&type8);
  auto featureMap1 = model->addOperand(&type20);
  auto in2 = model->addOperand(&type34);
  auto param27 = model->addOperand(&type7);
  auto out1 = model->addOperand(&type31);
  // Phase 2, operations
  static _Float16 scores1_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores1, scores1_init, sizeof(_Float16) * 2);
  static _Float16 roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(_Float16) * 8);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static _Float16 param15_init[] = {0.30000001192092896f};
  model->setOperandValue(param15, param15_init, sizeof(_Float16) * 1);
  static int32_t param16_init[] = {-1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static _Float16 param18_init[] = {0.4000000059604645f};
  model->setOperandValue(param18, param18_init, sizeof(_Float16) * 1);
  static _Float16 param19_init[] = {1.0f};
  model->setOperandValue(param19, param19_init, sizeof(_Float16) * 1);
  static _Float16 param20_init[] = {0.30000001192092896f};
  model->setOperandValue(param20, param20_init, sizeof(_Float16) * 1);
  static int32_t param21_init[] = {2};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static _Float16 param23_init[] = {2.0f};
  model->setOperandValue(param23, param23_init, sizeof(_Float16) * 1);
  static _Float16 param24_init[] = {2.0f};
  model->setOperandValue(param24, param24_init, sizeof(_Float16) * 1);
  static int32_t param25_init[] = {4};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {4};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static bool8 layout1_init[] = {false};
  model->setOperandValue(layout1, layout1_init, sizeof(bool8) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param14, param15, param16, param17, param18, param19, param20}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param21, param22, param23, param24, param25, param26, layout1}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_CONCATENATION, {featureMap1, in2, param27}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1, in2},
    {scoresOut1, classesOut1, out1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

