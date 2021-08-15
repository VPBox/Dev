// clang-format off
// Generated file (from: detection_postprocess.mod.py). Do not edit
void CreateModel_regular(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 6, 3});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 6, 4});
  OperandType type2(Type::TENSOR_FLOAT32, {6, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {1, 3});
  OperandType type4(Type::TENSOR_FLOAT32, {1, 3, 4});
  OperandType type5(Type::TENSOR_INT32, {1, 3});
  OperandType type6(Type::TENSOR_INT32, {1});
  OperandType type7(Type::FLOAT32, {});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type0);
  auto roi = model->addOperand(&type1);
  auto anchors = model->addOperand(&type2);
  auto param = model->addOperand(&type7);
  auto param1 = model->addOperand(&type7);
  auto param2 = model->addOperand(&type7);
  auto param3 = model->addOperand(&type7);
  auto param4 = model->addOperand(&type8);
  auto param5 = model->addOperand(&type9);
  auto param6 = model->addOperand(&type9);
  auto param7 = model->addOperand(&type9);
  auto param8 = model->addOperand(&type7);
  auto param9 = model->addOperand(&type7);
  auto param10 = model->addOperand(&type8);
  auto scoresOut = model->addOperand(&type3);
  auto roiOut = model->addOperand(&type4);
  auto classesOut = model->addOperand(&type5);
  auto detectionOut = model->addOperand(&type6);
  // Phase 2, operations
  static float param_init[] = {10.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  static float param1_init[] = {10.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {5.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {5.0f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static bool8 param4_init[] = {true};
  model->setOperandValue(param4, param4_init, sizeof(bool8) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {1};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static float param8_init[] = {0.0f};
  model->setOperandValue(param8, param8_init, sizeof(float) * 1);
  static float param9_init[] = {0.5f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static bool8 param10_init[] = {false};
  model->setOperandValue(param10, param10_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DETECTION_POSTPROCESSING, {scores, roi, anchors, param, param1, param2, param3, param4, param5, param6, param7, param8, param9, param10}, {scoresOut, roiOut, classesOut, detectionOut});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores, roi, anchors},
    {scoresOut, roiOut, classesOut, detectionOut});
  assert(model->isValid());
}

inline bool is_ignored_regular(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_regular_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 6, 3});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 6, 4});
  OperandType type2(Type::TENSOR_FLOAT32, {6, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {1, 3});
  OperandType type4(Type::TENSOR_FLOAT32, {1, 3, 4});
  OperandType type5(Type::TENSOR_INT32, {1, 3});
  OperandType type6(Type::TENSOR_INT32, {1});
  OperandType type7(Type::FLOAT32, {});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type0);
  auto roi = model->addOperand(&type1);
  auto anchors = model->addOperand(&type2);
  auto param = model->addOperand(&type7);
  auto param1 = model->addOperand(&type7);
  auto param2 = model->addOperand(&type7);
  auto param3 = model->addOperand(&type7);
  auto param4 = model->addOperand(&type8);
  auto param5 = model->addOperand(&type9);
  auto param6 = model->addOperand(&type9);
  auto param7 = model->addOperand(&type9);
  auto param8 = model->addOperand(&type7);
  auto param9 = model->addOperand(&type7);
  auto param10 = model->addOperand(&type8);
  auto scoresOut = model->addOperand(&type3);
  auto roiOut = model->addOperand(&type4);
  auto classesOut = model->addOperand(&type5);
  auto detectionOut = model->addOperand(&type6);
  // Phase 2, operations
  static float param_init[] = {10.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  static float param1_init[] = {10.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {5.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {5.0f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static bool8 param4_init[] = {true};
  model->setOperandValue(param4, param4_init, sizeof(bool8) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {1};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static float param8_init[] = {0.0f};
  model->setOperandValue(param8, param8_init, sizeof(float) * 1);
  static float param9_init[] = {0.5f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static bool8 param10_init[] = {false};
  model->setOperandValue(param10, param10_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DETECTION_POSTPROCESSING, {scores, roi, anchors, param, param1, param2, param3, param4, param5, param6, param7, param8, param9, param10}, {scoresOut, roiOut, classesOut, detectionOut});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores, roi, anchors},
    {scoresOut, roiOut, classesOut, detectionOut});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_regular_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_regular_float16(Model *model) {
  OperandType type11(Type::TENSOR_FLOAT16, {6, 4});
  OperandType type12(Type::FLOAT16, {});
  OperandType type13(Type::TENSOR_FLOAT16, {1, 6, 4});
  OperandType type14(Type::TENSOR_FLOAT16, {1, 3, 4});
  OperandType type15(Type::TENSOR_FLOAT16, {1, 6, 3});
  OperandType type16(Type::TENSOR_FLOAT16, {1, 3});
  OperandType type5(Type::TENSOR_INT32, {1, 3});
  OperandType type6(Type::TENSOR_INT32, {1});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type15);
  auto roi = model->addOperand(&type13);
  auto anchors = model->addOperand(&type11);
  auto param = model->addOperand(&type12);
  auto param1 = model->addOperand(&type12);
  auto param2 = model->addOperand(&type12);
  auto param3 = model->addOperand(&type12);
  auto param4 = model->addOperand(&type8);
  auto param5 = model->addOperand(&type9);
  auto param6 = model->addOperand(&type9);
  auto param7 = model->addOperand(&type9);
  auto param8 = model->addOperand(&type12);
  auto param9 = model->addOperand(&type12);
  auto param10 = model->addOperand(&type8);
  auto scoresOut = model->addOperand(&type16);
  auto roiOut = model->addOperand(&type14);
  auto classesOut = model->addOperand(&type5);
  auto detectionOut = model->addOperand(&type6);
  // Phase 2, operations
  static _Float16 param_init[] = {10.0f};
  model->setOperandValue(param, param_init, sizeof(_Float16) * 1);
  static _Float16 param1_init[] = {10.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {5.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {5.0f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static bool8 param4_init[] = {true};
  model->setOperandValue(param4, param4_init, sizeof(bool8) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {1};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static _Float16 param8_init[] = {0.0f};
  model->setOperandValue(param8, param8_init, sizeof(_Float16) * 1);
  static _Float16 param9_init[] = {0.5f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static bool8 param10_init[] = {false};
  model->setOperandValue(param10, param10_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DETECTION_POSTPROCESSING, {scores, roi, anchors, param, param1, param2, param3, param4, param5, param6, param7, param8, param9, param10}, {scoresOut, roiOut, classesOut, detectionOut});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores, roi, anchors},
    {scoresOut, roiOut, classesOut, detectionOut});
  assert(model->isValid());
}

inline bool is_ignored_regular_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_regular_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 6, 3});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 6, 4});
  OperandType type17(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type18(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type19(Type::TENSOR_INT32, {0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {6, 4});
  OperandType type20(Type::TENSOR_INT32, {0});
  OperandType type7(Type::FLOAT32, {});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type0);
  auto roi = model->addOperand(&type1);
  auto anchors = model->addOperand(&type2);
  auto param = model->addOperand(&type7);
  auto param1 = model->addOperand(&type7);
  auto param2 = model->addOperand(&type7);
  auto param3 = model->addOperand(&type7);
  auto param4 = model->addOperand(&type8);
  auto param5 = model->addOperand(&type9);
  auto param6 = model->addOperand(&type9);
  auto param7 = model->addOperand(&type9);
  auto param8 = model->addOperand(&type7);
  auto param9 = model->addOperand(&type7);
  auto param10 = model->addOperand(&type8);
  auto scoresOut = model->addOperand(&type17);
  auto roiOut = model->addOperand(&type18);
  auto classesOut = model->addOperand(&type19);
  auto detectionOut = model->addOperand(&type20);
  // Phase 2, operations
  static float param_init[] = {10.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  static float param1_init[] = {10.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {5.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {5.0f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static bool8 param4_init[] = {true};
  model->setOperandValue(param4, param4_init, sizeof(bool8) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {1};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static float param8_init[] = {0.0f};
  model->setOperandValue(param8, param8_init, sizeof(float) * 1);
  static float param9_init[] = {0.5f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static bool8 param10_init[] = {false};
  model->setOperandValue(param10, param10_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DETECTION_POSTPROCESSING, {scores, roi, anchors, param, param1, param2, param3, param4, param5, param6, param7, param8, param9, param10}, {scoresOut, roiOut, classesOut, detectionOut});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores, roi, anchors},
    {scoresOut, roiOut, classesOut, detectionOut});
  assert(model->isValid());
}

inline bool is_ignored_regular_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_regular_dynamic_output_shape_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 6, 3});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 6, 4});
  OperandType type17(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type18(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type19(Type::TENSOR_INT32, {0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {6, 4});
  OperandType type20(Type::TENSOR_INT32, {0});
  OperandType type7(Type::FLOAT32, {});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type0);
  auto roi = model->addOperand(&type1);
  auto anchors = model->addOperand(&type2);
  auto param = model->addOperand(&type7);
  auto param1 = model->addOperand(&type7);
  auto param2 = model->addOperand(&type7);
  auto param3 = model->addOperand(&type7);
  auto param4 = model->addOperand(&type8);
  auto param5 = model->addOperand(&type9);
  auto param6 = model->addOperand(&type9);
  auto param7 = model->addOperand(&type9);
  auto param8 = model->addOperand(&type7);
  auto param9 = model->addOperand(&type7);
  auto param10 = model->addOperand(&type8);
  auto scoresOut = model->addOperand(&type17);
  auto roiOut = model->addOperand(&type18);
  auto classesOut = model->addOperand(&type19);
  auto detectionOut = model->addOperand(&type20);
  // Phase 2, operations
  static float param_init[] = {10.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  static float param1_init[] = {10.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {5.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {5.0f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static bool8 param4_init[] = {true};
  model->setOperandValue(param4, param4_init, sizeof(bool8) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {1};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static float param8_init[] = {0.0f};
  model->setOperandValue(param8, param8_init, sizeof(float) * 1);
  static float param9_init[] = {0.5f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static bool8 param10_init[] = {false};
  model->setOperandValue(param10, param10_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DETECTION_POSTPROCESSING, {scores, roi, anchors, param, param1, param2, param3, param4, param5, param6, param7, param8, param9, param10}, {scoresOut, roiOut, classesOut, detectionOut});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores, roi, anchors},
    {scoresOut, roiOut, classesOut, detectionOut});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_regular_dynamic_output_shape_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_regular_dynamic_output_shape_float16(Model *model) {
  OperandType type11(Type::TENSOR_FLOAT16, {6, 4});
  OperandType type12(Type::FLOAT16, {});
  OperandType type13(Type::TENSOR_FLOAT16, {1, 6, 4});
  OperandType type15(Type::TENSOR_FLOAT16, {1, 6, 3});
  OperandType type19(Type::TENSOR_INT32, {0, 0});
  OperandType type20(Type::TENSOR_INT32, {0});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type22(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type15);
  auto roi = model->addOperand(&type13);
  auto anchors = model->addOperand(&type11);
  auto param = model->addOperand(&type12);
  auto param1 = model->addOperand(&type12);
  auto param2 = model->addOperand(&type12);
  auto param3 = model->addOperand(&type12);
  auto param4 = model->addOperand(&type8);
  auto param5 = model->addOperand(&type9);
  auto param6 = model->addOperand(&type9);
  auto param7 = model->addOperand(&type9);
  auto param8 = model->addOperand(&type12);
  auto param9 = model->addOperand(&type12);
  auto param10 = model->addOperand(&type8);
  auto scoresOut = model->addOperand(&type21);
  auto roiOut = model->addOperand(&type22);
  auto classesOut = model->addOperand(&type19);
  auto detectionOut = model->addOperand(&type20);
  // Phase 2, operations
  static _Float16 param_init[] = {10.0f};
  model->setOperandValue(param, param_init, sizeof(_Float16) * 1);
  static _Float16 param1_init[] = {10.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {5.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {5.0f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static bool8 param4_init[] = {true};
  model->setOperandValue(param4, param4_init, sizeof(bool8) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {1};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static _Float16 param8_init[] = {0.0f};
  model->setOperandValue(param8, param8_init, sizeof(_Float16) * 1);
  static _Float16 param9_init[] = {0.5f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static bool8 param10_init[] = {false};
  model->setOperandValue(param10, param10_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DETECTION_POSTPROCESSING, {scores, roi, anchors, param, param1, param2, param3, param4, param5, param6, param7, param8, param9, param10}, {scoresOut, roiOut, classesOut, detectionOut});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores, roi, anchors},
    {scoresOut, roiOut, classesOut, detectionOut});
  assert(model->isValid());
}

inline bool is_ignored_regular_dynamic_output_shape_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 6, 3});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 6, 4});
  OperandType type2(Type::TENSOR_FLOAT32, {6, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {1, 3});
  OperandType type4(Type::TENSOR_FLOAT32, {1, 3, 4});
  OperandType type5(Type::TENSOR_INT32, {1, 3});
  OperandType type6(Type::TENSOR_INT32, {1});
  OperandType type7(Type::FLOAT32, {});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type0);
  auto roi1 = model->addOperand(&type1);
  auto anchors1 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type7);
  auto param12 = model->addOperand(&type7);
  auto param13 = model->addOperand(&type7);
  auto param14 = model->addOperand(&type7);
  auto param15 = model->addOperand(&type8);
  auto param16 = model->addOperand(&type9);
  auto param17 = model->addOperand(&type9);
  auto param18 = model->addOperand(&type9);
  auto param19 = model->addOperand(&type7);
  auto param20 = model->addOperand(&type7);
  auto param21 = model->addOperand(&type8);
  auto scoresOut1 = model->addOperand(&type3);
  auto roiOut1 = model->addOperand(&type4);
  auto classesOut1 = model->addOperand(&type5);
  auto detectionOut1 = model->addOperand(&type6);
  // Phase 2, operations
  static float param11_init[] = {10.0f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static float param12_init[] = {10.0f};
  model->setOperandValue(param12, param12_init, sizeof(float) * 1);
  static float param13_init[] = {5.0f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static float param14_init[] = {5.0f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static bool8 param15_init[] = {false};
  model->setOperandValue(param15, param15_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static float param19_init[] = {0.0f};
  model->setOperandValue(param19, param19_init, sizeof(float) * 1);
  static float param20_init[] = {0.5f};
  model->setOperandValue(param20, param20_init, sizeof(float) * 1);
  static bool8 param21_init[] = {false};
  model->setOperandValue(param21, param21_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DETECTION_POSTPROCESSING, {scores1, roi1, anchors1, param11, param12, param13, param14, param15, param16, param17, param18, param19, param20, param21}, {scoresOut1, roiOut1, classesOut1, detectionOut1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores1, roi1, anchors1},
    {scoresOut1, roiOut1, classesOut1, detectionOut1});
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 6, 3});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 6, 4});
  OperandType type2(Type::TENSOR_FLOAT32, {6, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {1, 3});
  OperandType type4(Type::TENSOR_FLOAT32, {1, 3, 4});
  OperandType type5(Type::TENSOR_INT32, {1, 3});
  OperandType type6(Type::TENSOR_INT32, {1});
  OperandType type7(Type::FLOAT32, {});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type0);
  auto roi1 = model->addOperand(&type1);
  auto anchors1 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type7);
  auto param12 = model->addOperand(&type7);
  auto param13 = model->addOperand(&type7);
  auto param14 = model->addOperand(&type7);
  auto param15 = model->addOperand(&type8);
  auto param16 = model->addOperand(&type9);
  auto param17 = model->addOperand(&type9);
  auto param18 = model->addOperand(&type9);
  auto param19 = model->addOperand(&type7);
  auto param20 = model->addOperand(&type7);
  auto param21 = model->addOperand(&type8);
  auto scoresOut1 = model->addOperand(&type3);
  auto roiOut1 = model->addOperand(&type4);
  auto classesOut1 = model->addOperand(&type5);
  auto detectionOut1 = model->addOperand(&type6);
  // Phase 2, operations
  static float param11_init[] = {10.0f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static float param12_init[] = {10.0f};
  model->setOperandValue(param12, param12_init, sizeof(float) * 1);
  static float param13_init[] = {5.0f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static float param14_init[] = {5.0f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static bool8 param15_init[] = {false};
  model->setOperandValue(param15, param15_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static float param19_init[] = {0.0f};
  model->setOperandValue(param19, param19_init, sizeof(float) * 1);
  static float param20_init[] = {0.5f};
  model->setOperandValue(param20, param20_init, sizeof(float) * 1);
  static bool8 param21_init[] = {false};
  model->setOperandValue(param21, param21_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DETECTION_POSTPROCESSING, {scores1, roi1, anchors1, param11, param12, param13, param14, param15, param16, param17, param18, param19, param20, param21}, {scoresOut1, roiOut1, classesOut1, detectionOut1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores1, roi1, anchors1},
    {scoresOut1, roiOut1, classesOut1, detectionOut1});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16(Model *model) {
  OperandType type11(Type::TENSOR_FLOAT16, {6, 4});
  OperandType type12(Type::FLOAT16, {});
  OperandType type13(Type::TENSOR_FLOAT16, {1, 6, 4});
  OperandType type14(Type::TENSOR_FLOAT16, {1, 3, 4});
  OperandType type15(Type::TENSOR_FLOAT16, {1, 6, 3});
  OperandType type16(Type::TENSOR_FLOAT16, {1, 3});
  OperandType type5(Type::TENSOR_INT32, {1, 3});
  OperandType type6(Type::TENSOR_INT32, {1});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type15);
  auto roi1 = model->addOperand(&type13);
  auto anchors1 = model->addOperand(&type11);
  auto param11 = model->addOperand(&type12);
  auto param12 = model->addOperand(&type12);
  auto param13 = model->addOperand(&type12);
  auto param14 = model->addOperand(&type12);
  auto param15 = model->addOperand(&type8);
  auto param16 = model->addOperand(&type9);
  auto param17 = model->addOperand(&type9);
  auto param18 = model->addOperand(&type9);
  auto param19 = model->addOperand(&type12);
  auto param20 = model->addOperand(&type12);
  auto param21 = model->addOperand(&type8);
  auto scoresOut1 = model->addOperand(&type16);
  auto roiOut1 = model->addOperand(&type14);
  auto classesOut1 = model->addOperand(&type5);
  auto detectionOut1 = model->addOperand(&type6);
  // Phase 2, operations
  static _Float16 param11_init[] = {10.0f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static _Float16 param12_init[] = {10.0f};
  model->setOperandValue(param12, param12_init, sizeof(_Float16) * 1);
  static _Float16 param13_init[] = {5.0f};
  model->setOperandValue(param13, param13_init, sizeof(_Float16) * 1);
  static _Float16 param14_init[] = {5.0f};
  model->setOperandValue(param14, param14_init, sizeof(_Float16) * 1);
  static bool8 param15_init[] = {false};
  model->setOperandValue(param15, param15_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static _Float16 param19_init[] = {0.0f};
  model->setOperandValue(param19, param19_init, sizeof(_Float16) * 1);
  static _Float16 param20_init[] = {0.5f};
  model->setOperandValue(param20, param20_init, sizeof(_Float16) * 1);
  static bool8 param21_init[] = {false};
  model->setOperandValue(param21, param21_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DETECTION_POSTPROCESSING, {scores1, roi1, anchors1, param11, param12, param13, param14, param15, param16, param17, param18, param19, param20, param21}, {scoresOut1, roiOut1, classesOut1, detectionOut1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores1, roi1, anchors1},
    {scoresOut1, roiOut1, classesOut1, detectionOut1});
  assert(model->isValid());
}

inline bool is_ignored_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 6, 3});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 6, 4});
  OperandType type17(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type18(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type19(Type::TENSOR_INT32, {0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {6, 4});
  OperandType type20(Type::TENSOR_INT32, {0});
  OperandType type7(Type::FLOAT32, {});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type0);
  auto roi1 = model->addOperand(&type1);
  auto anchors1 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type7);
  auto param12 = model->addOperand(&type7);
  auto param13 = model->addOperand(&type7);
  auto param14 = model->addOperand(&type7);
  auto param15 = model->addOperand(&type8);
  auto param16 = model->addOperand(&type9);
  auto param17 = model->addOperand(&type9);
  auto param18 = model->addOperand(&type9);
  auto param19 = model->addOperand(&type7);
  auto param20 = model->addOperand(&type7);
  auto param21 = model->addOperand(&type8);
  auto scoresOut1 = model->addOperand(&type17);
  auto roiOut1 = model->addOperand(&type18);
  auto classesOut1 = model->addOperand(&type19);
  auto detectionOut1 = model->addOperand(&type20);
  // Phase 2, operations
  static float param11_init[] = {10.0f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static float param12_init[] = {10.0f};
  model->setOperandValue(param12, param12_init, sizeof(float) * 1);
  static float param13_init[] = {5.0f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static float param14_init[] = {5.0f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static bool8 param15_init[] = {false};
  model->setOperandValue(param15, param15_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static float param19_init[] = {0.0f};
  model->setOperandValue(param19, param19_init, sizeof(float) * 1);
  static float param20_init[] = {0.5f};
  model->setOperandValue(param20, param20_init, sizeof(float) * 1);
  static bool8 param21_init[] = {false};
  model->setOperandValue(param21, param21_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DETECTION_POSTPROCESSING, {scores1, roi1, anchors1, param11, param12, param13, param14, param15, param16, param17, param18, param19, param20, param21}, {scoresOut1, roiOut1, classesOut1, detectionOut1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores1, roi1, anchors1},
    {scoresOut1, roiOut1, classesOut1, detectionOut1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 6, 3});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 6, 4});
  OperandType type17(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type18(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type19(Type::TENSOR_INT32, {0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {6, 4});
  OperandType type20(Type::TENSOR_INT32, {0});
  OperandType type7(Type::FLOAT32, {});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type0);
  auto roi1 = model->addOperand(&type1);
  auto anchors1 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type7);
  auto param12 = model->addOperand(&type7);
  auto param13 = model->addOperand(&type7);
  auto param14 = model->addOperand(&type7);
  auto param15 = model->addOperand(&type8);
  auto param16 = model->addOperand(&type9);
  auto param17 = model->addOperand(&type9);
  auto param18 = model->addOperand(&type9);
  auto param19 = model->addOperand(&type7);
  auto param20 = model->addOperand(&type7);
  auto param21 = model->addOperand(&type8);
  auto scoresOut1 = model->addOperand(&type17);
  auto roiOut1 = model->addOperand(&type18);
  auto classesOut1 = model->addOperand(&type19);
  auto detectionOut1 = model->addOperand(&type20);
  // Phase 2, operations
  static float param11_init[] = {10.0f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static float param12_init[] = {10.0f};
  model->setOperandValue(param12, param12_init, sizeof(float) * 1);
  static float param13_init[] = {5.0f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static float param14_init[] = {5.0f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static bool8 param15_init[] = {false};
  model->setOperandValue(param15, param15_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static float param19_init[] = {0.0f};
  model->setOperandValue(param19, param19_init, sizeof(float) * 1);
  static float param20_init[] = {0.5f};
  model->setOperandValue(param20, param20_init, sizeof(float) * 1);
  static bool8 param21_init[] = {false};
  model->setOperandValue(param21, param21_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DETECTION_POSTPROCESSING, {scores1, roi1, anchors1, param11, param12, param13, param14, param15, param16, param17, param18, param19, param20, param21}, {scoresOut1, roiOut1, classesOut1, detectionOut1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores1, roi1, anchors1},
    {scoresOut1, roiOut1, classesOut1, detectionOut1});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16(Model *model) {
  OperandType type11(Type::TENSOR_FLOAT16, {6, 4});
  OperandType type12(Type::FLOAT16, {});
  OperandType type13(Type::TENSOR_FLOAT16, {1, 6, 4});
  OperandType type15(Type::TENSOR_FLOAT16, {1, 6, 3});
  OperandType type19(Type::TENSOR_INT32, {0, 0});
  OperandType type20(Type::TENSOR_INT32, {0});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type22(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type15);
  auto roi1 = model->addOperand(&type13);
  auto anchors1 = model->addOperand(&type11);
  auto param11 = model->addOperand(&type12);
  auto param12 = model->addOperand(&type12);
  auto param13 = model->addOperand(&type12);
  auto param14 = model->addOperand(&type12);
  auto param15 = model->addOperand(&type8);
  auto param16 = model->addOperand(&type9);
  auto param17 = model->addOperand(&type9);
  auto param18 = model->addOperand(&type9);
  auto param19 = model->addOperand(&type12);
  auto param20 = model->addOperand(&type12);
  auto param21 = model->addOperand(&type8);
  auto scoresOut1 = model->addOperand(&type21);
  auto roiOut1 = model->addOperand(&type22);
  auto classesOut1 = model->addOperand(&type19);
  auto detectionOut1 = model->addOperand(&type20);
  // Phase 2, operations
  static _Float16 param11_init[] = {10.0f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static _Float16 param12_init[] = {10.0f};
  model->setOperandValue(param12, param12_init, sizeof(_Float16) * 1);
  static _Float16 param13_init[] = {5.0f};
  model->setOperandValue(param13, param13_init, sizeof(_Float16) * 1);
  static _Float16 param14_init[] = {5.0f};
  model->setOperandValue(param14, param14_init, sizeof(_Float16) * 1);
  static bool8 param15_init[] = {false};
  model->setOperandValue(param15, param15_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static _Float16 param19_init[] = {0.0f};
  model->setOperandValue(param19, param19_init, sizeof(_Float16) * 1);
  static _Float16 param20_init[] = {0.5f};
  model->setOperandValue(param20, param20_init, sizeof(_Float16) * 1);
  static bool8 param21_init[] = {false};
  model->setOperandValue(param21, param21_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DETECTION_POSTPROCESSING, {scores1, roi1, anchors1, param11, param12, param13, param14, param15, param16, param17, param18, param19, param20, param21}, {scoresOut1, roiOut1, classesOut1, detectionOut1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores1, roi1, anchors1},
    {scoresOut1, roiOut1, classesOut1, detectionOut1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 6, 3});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 6, 7});
  OperandType type2(Type::TENSOR_FLOAT32, {6, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {1, 3});
  OperandType type4(Type::TENSOR_FLOAT32, {1, 3, 4});
  OperandType type5(Type::TENSOR_INT32, {1, 3});
  OperandType type6(Type::TENSOR_INT32, {1});
  OperandType type7(Type::FLOAT32, {});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores2 = model->addOperand(&type0);
  auto roi2 = model->addOperand(&type10);
  auto anchors2 = model->addOperand(&type2);
  auto param22 = model->addOperand(&type7);
  auto param23 = model->addOperand(&type7);
  auto param24 = model->addOperand(&type7);
  auto param25 = model->addOperand(&type7);
  auto param26 = model->addOperand(&type8);
  auto param27 = model->addOperand(&type9);
  auto param28 = model->addOperand(&type9);
  auto param29 = model->addOperand(&type9);
  auto param30 = model->addOperand(&type7);
  auto param31 = model->addOperand(&type7);
  auto param32 = model->addOperand(&type8);
  auto scoresOut2 = model->addOperand(&type3);
  auto roiOut2 = model->addOperand(&type4);
  auto classesOut2 = model->addOperand(&type5);
  auto detectionOut2 = model->addOperand(&type6);
  // Phase 2, operations
  static float param22_init[] = {10.0f};
  model->setOperandValue(param22, param22_init, sizeof(float) * 1);
  static float param23_init[] = {10.0f};
  model->setOperandValue(param23, param23_init, sizeof(float) * 1);
  static float param24_init[] = {5.0f};
  model->setOperandValue(param24, param24_init, sizeof(float) * 1);
  static float param25_init[] = {5.0f};
  model->setOperandValue(param25, param25_init, sizeof(float) * 1);
  static bool8 param26_init[] = {false};
  model->setOperandValue(param26, param26_init, sizeof(bool8) * 1);
  static int32_t param27_init[] = {3};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {1};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {1};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static float param30_init[] = {0.0f};
  model->setOperandValue(param30, param30_init, sizeof(float) * 1);
  static float param31_init[] = {0.5f};
  model->setOperandValue(param31, param31_init, sizeof(float) * 1);
  static bool8 param32_init[] = {false};
  model->setOperandValue(param32, param32_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DETECTION_POSTPROCESSING, {scores2, roi2, anchors2, param22, param23, param24, param25, param26, param27, param28, param29, param30, param31, param32}, {scoresOut2, roiOut2, classesOut2, detectionOut2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores2, roi2, anchors2},
    {scoresOut2, roiOut2, classesOut2, detectionOut2});
  assert(model->isValid());
}

inline bool is_ignored_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 6, 3});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 6, 7});
  OperandType type2(Type::TENSOR_FLOAT32, {6, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {1, 3});
  OperandType type4(Type::TENSOR_FLOAT32, {1, 3, 4});
  OperandType type5(Type::TENSOR_INT32, {1, 3});
  OperandType type6(Type::TENSOR_INT32, {1});
  OperandType type7(Type::FLOAT32, {});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores2 = model->addOperand(&type0);
  auto roi2 = model->addOperand(&type10);
  auto anchors2 = model->addOperand(&type2);
  auto param22 = model->addOperand(&type7);
  auto param23 = model->addOperand(&type7);
  auto param24 = model->addOperand(&type7);
  auto param25 = model->addOperand(&type7);
  auto param26 = model->addOperand(&type8);
  auto param27 = model->addOperand(&type9);
  auto param28 = model->addOperand(&type9);
  auto param29 = model->addOperand(&type9);
  auto param30 = model->addOperand(&type7);
  auto param31 = model->addOperand(&type7);
  auto param32 = model->addOperand(&type8);
  auto scoresOut2 = model->addOperand(&type3);
  auto roiOut2 = model->addOperand(&type4);
  auto classesOut2 = model->addOperand(&type5);
  auto detectionOut2 = model->addOperand(&type6);
  // Phase 2, operations
  static float param22_init[] = {10.0f};
  model->setOperandValue(param22, param22_init, sizeof(float) * 1);
  static float param23_init[] = {10.0f};
  model->setOperandValue(param23, param23_init, sizeof(float) * 1);
  static float param24_init[] = {5.0f};
  model->setOperandValue(param24, param24_init, sizeof(float) * 1);
  static float param25_init[] = {5.0f};
  model->setOperandValue(param25, param25_init, sizeof(float) * 1);
  static bool8 param26_init[] = {false};
  model->setOperandValue(param26, param26_init, sizeof(bool8) * 1);
  static int32_t param27_init[] = {3};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {1};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {1};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static float param30_init[] = {0.0f};
  model->setOperandValue(param30, param30_init, sizeof(float) * 1);
  static float param31_init[] = {0.5f};
  model->setOperandValue(param31, param31_init, sizeof(float) * 1);
  static bool8 param32_init[] = {false};
  model->setOperandValue(param32, param32_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DETECTION_POSTPROCESSING, {scores2, roi2, anchors2, param22, param23, param24, param25, param26, param27, param28, param29, param30, param31, param32}, {scoresOut2, roiOut2, classesOut2, detectionOut2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores2, roi2, anchors2},
    {scoresOut2, roiOut2, classesOut2, detectionOut2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_2(Model *model) {
  OperandType type11(Type::TENSOR_FLOAT16, {6, 4});
  OperandType type12(Type::FLOAT16, {});
  OperandType type14(Type::TENSOR_FLOAT16, {1, 3, 4});
  OperandType type15(Type::TENSOR_FLOAT16, {1, 6, 3});
  OperandType type16(Type::TENSOR_FLOAT16, {1, 3});
  OperandType type23(Type::TENSOR_FLOAT16, {1, 6, 7});
  OperandType type5(Type::TENSOR_INT32, {1, 3});
  OperandType type6(Type::TENSOR_INT32, {1});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores2 = model->addOperand(&type15);
  auto roi2 = model->addOperand(&type23);
  auto anchors2 = model->addOperand(&type11);
  auto param22 = model->addOperand(&type12);
  auto param23 = model->addOperand(&type12);
  auto param24 = model->addOperand(&type12);
  auto param25 = model->addOperand(&type12);
  auto param26 = model->addOperand(&type8);
  auto param27 = model->addOperand(&type9);
  auto param28 = model->addOperand(&type9);
  auto param29 = model->addOperand(&type9);
  auto param30 = model->addOperand(&type12);
  auto param31 = model->addOperand(&type12);
  auto param32 = model->addOperand(&type8);
  auto scoresOut2 = model->addOperand(&type16);
  auto roiOut2 = model->addOperand(&type14);
  auto classesOut2 = model->addOperand(&type5);
  auto detectionOut2 = model->addOperand(&type6);
  // Phase 2, operations
  static _Float16 param22_init[] = {10.0f};
  model->setOperandValue(param22, param22_init, sizeof(_Float16) * 1);
  static _Float16 param23_init[] = {10.0f};
  model->setOperandValue(param23, param23_init, sizeof(_Float16) * 1);
  static _Float16 param24_init[] = {5.0f};
  model->setOperandValue(param24, param24_init, sizeof(_Float16) * 1);
  static _Float16 param25_init[] = {5.0f};
  model->setOperandValue(param25, param25_init, sizeof(_Float16) * 1);
  static bool8 param26_init[] = {false};
  model->setOperandValue(param26, param26_init, sizeof(bool8) * 1);
  static int32_t param27_init[] = {3};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {1};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {1};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static _Float16 param30_init[] = {0.0f};
  model->setOperandValue(param30, param30_init, sizeof(_Float16) * 1);
  static _Float16 param31_init[] = {0.5f};
  model->setOperandValue(param31, param31_init, sizeof(_Float16) * 1);
  static bool8 param32_init[] = {false};
  model->setOperandValue(param32, param32_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DETECTION_POSTPROCESSING, {scores2, roi2, anchors2, param22, param23, param24, param25, param26, param27, param28, param29, param30, param31, param32}, {scoresOut2, roiOut2, classesOut2, detectionOut2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores2, roi2, anchors2},
    {scoresOut2, roiOut2, classesOut2, detectionOut2});
  assert(model->isValid());
}

inline bool is_ignored_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 6, 3});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 6, 7});
  OperandType type17(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type18(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type19(Type::TENSOR_INT32, {0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {6, 4});
  OperandType type20(Type::TENSOR_INT32, {0});
  OperandType type7(Type::FLOAT32, {});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores2 = model->addOperand(&type0);
  auto roi2 = model->addOperand(&type10);
  auto anchors2 = model->addOperand(&type2);
  auto param22 = model->addOperand(&type7);
  auto param23 = model->addOperand(&type7);
  auto param24 = model->addOperand(&type7);
  auto param25 = model->addOperand(&type7);
  auto param26 = model->addOperand(&type8);
  auto param27 = model->addOperand(&type9);
  auto param28 = model->addOperand(&type9);
  auto param29 = model->addOperand(&type9);
  auto param30 = model->addOperand(&type7);
  auto param31 = model->addOperand(&type7);
  auto param32 = model->addOperand(&type8);
  auto scoresOut2 = model->addOperand(&type17);
  auto roiOut2 = model->addOperand(&type18);
  auto classesOut2 = model->addOperand(&type19);
  auto detectionOut2 = model->addOperand(&type20);
  // Phase 2, operations
  static float param22_init[] = {10.0f};
  model->setOperandValue(param22, param22_init, sizeof(float) * 1);
  static float param23_init[] = {10.0f};
  model->setOperandValue(param23, param23_init, sizeof(float) * 1);
  static float param24_init[] = {5.0f};
  model->setOperandValue(param24, param24_init, sizeof(float) * 1);
  static float param25_init[] = {5.0f};
  model->setOperandValue(param25, param25_init, sizeof(float) * 1);
  static bool8 param26_init[] = {false};
  model->setOperandValue(param26, param26_init, sizeof(bool8) * 1);
  static int32_t param27_init[] = {3};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {1};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {1};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static float param30_init[] = {0.0f};
  model->setOperandValue(param30, param30_init, sizeof(float) * 1);
  static float param31_init[] = {0.5f};
  model->setOperandValue(param31, param31_init, sizeof(float) * 1);
  static bool8 param32_init[] = {false};
  model->setOperandValue(param32, param32_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DETECTION_POSTPROCESSING, {scores2, roi2, anchors2, param22, param23, param24, param25, param26, param27, param28, param29, param30, param31, param32}, {scoresOut2, roiOut2, classesOut2, detectionOut2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores2, roi2, anchors2},
    {scoresOut2, roiOut2, classesOut2, detectionOut2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 6, 3});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 6, 7});
  OperandType type17(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type18(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type19(Type::TENSOR_INT32, {0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {6, 4});
  OperandType type20(Type::TENSOR_INT32, {0});
  OperandType type7(Type::FLOAT32, {});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores2 = model->addOperand(&type0);
  auto roi2 = model->addOperand(&type10);
  auto anchors2 = model->addOperand(&type2);
  auto param22 = model->addOperand(&type7);
  auto param23 = model->addOperand(&type7);
  auto param24 = model->addOperand(&type7);
  auto param25 = model->addOperand(&type7);
  auto param26 = model->addOperand(&type8);
  auto param27 = model->addOperand(&type9);
  auto param28 = model->addOperand(&type9);
  auto param29 = model->addOperand(&type9);
  auto param30 = model->addOperand(&type7);
  auto param31 = model->addOperand(&type7);
  auto param32 = model->addOperand(&type8);
  auto scoresOut2 = model->addOperand(&type17);
  auto roiOut2 = model->addOperand(&type18);
  auto classesOut2 = model->addOperand(&type19);
  auto detectionOut2 = model->addOperand(&type20);
  // Phase 2, operations
  static float param22_init[] = {10.0f};
  model->setOperandValue(param22, param22_init, sizeof(float) * 1);
  static float param23_init[] = {10.0f};
  model->setOperandValue(param23, param23_init, sizeof(float) * 1);
  static float param24_init[] = {5.0f};
  model->setOperandValue(param24, param24_init, sizeof(float) * 1);
  static float param25_init[] = {5.0f};
  model->setOperandValue(param25, param25_init, sizeof(float) * 1);
  static bool8 param26_init[] = {false};
  model->setOperandValue(param26, param26_init, sizeof(bool8) * 1);
  static int32_t param27_init[] = {3};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {1};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {1};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static float param30_init[] = {0.0f};
  model->setOperandValue(param30, param30_init, sizeof(float) * 1);
  static float param31_init[] = {0.5f};
  model->setOperandValue(param31, param31_init, sizeof(float) * 1);
  static bool8 param32_init[] = {false};
  model->setOperandValue(param32, param32_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DETECTION_POSTPROCESSING, {scores2, roi2, anchors2, param22, param23, param24, param25, param26, param27, param28, param29, param30, param31, param32}, {scoresOut2, roiOut2, classesOut2, detectionOut2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores2, roi2, anchors2},
    {scoresOut2, roiOut2, classesOut2, detectionOut2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_2(Model *model) {
  OperandType type11(Type::TENSOR_FLOAT16, {6, 4});
  OperandType type12(Type::FLOAT16, {});
  OperandType type15(Type::TENSOR_FLOAT16, {1, 6, 3});
  OperandType type19(Type::TENSOR_INT32, {0, 0});
  OperandType type20(Type::TENSOR_INT32, {0});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type22(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type23(Type::TENSOR_FLOAT16, {1, 6, 7});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores2 = model->addOperand(&type15);
  auto roi2 = model->addOperand(&type23);
  auto anchors2 = model->addOperand(&type11);
  auto param22 = model->addOperand(&type12);
  auto param23 = model->addOperand(&type12);
  auto param24 = model->addOperand(&type12);
  auto param25 = model->addOperand(&type12);
  auto param26 = model->addOperand(&type8);
  auto param27 = model->addOperand(&type9);
  auto param28 = model->addOperand(&type9);
  auto param29 = model->addOperand(&type9);
  auto param30 = model->addOperand(&type12);
  auto param31 = model->addOperand(&type12);
  auto param32 = model->addOperand(&type8);
  auto scoresOut2 = model->addOperand(&type21);
  auto roiOut2 = model->addOperand(&type22);
  auto classesOut2 = model->addOperand(&type19);
  auto detectionOut2 = model->addOperand(&type20);
  // Phase 2, operations
  static _Float16 param22_init[] = {10.0f};
  model->setOperandValue(param22, param22_init, sizeof(_Float16) * 1);
  static _Float16 param23_init[] = {10.0f};
  model->setOperandValue(param23, param23_init, sizeof(_Float16) * 1);
  static _Float16 param24_init[] = {5.0f};
  model->setOperandValue(param24, param24_init, sizeof(_Float16) * 1);
  static _Float16 param25_init[] = {5.0f};
  model->setOperandValue(param25, param25_init, sizeof(_Float16) * 1);
  static bool8 param26_init[] = {false};
  model->setOperandValue(param26, param26_init, sizeof(bool8) * 1);
  static int32_t param27_init[] = {3};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {1};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {1};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static _Float16 param30_init[] = {0.0f};
  model->setOperandValue(param30, param30_init, sizeof(_Float16) * 1);
  static _Float16 param31_init[] = {0.5f};
  model->setOperandValue(param31, param31_init, sizeof(_Float16) * 1);
  static bool8 param32_init[] = {false};
  model->setOperandValue(param32, param32_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DETECTION_POSTPROCESSING, {scores2, roi2, anchors2, param22, param23, param24, param25, param26, param27, param28, param29, param30, param31, param32}, {scoresOut2, roiOut2, classesOut2, detectionOut2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores2, roi2, anchors2},
    {scoresOut2, roiOut2, classesOut2, detectionOut2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 6, 3});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 6, 7});
  OperandType type2(Type::TENSOR_FLOAT32, {6, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {1, 3});
  OperandType type4(Type::TENSOR_FLOAT32, {1, 3, 4});
  OperandType type5(Type::TENSOR_INT32, {1, 3});
  OperandType type6(Type::TENSOR_INT32, {1});
  OperandType type7(Type::FLOAT32, {});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores3 = model->addOperand(&type0);
  auto roi3 = model->addOperand(&type10);
  auto anchors3 = model->addOperand(&type2);
  auto param33 = model->addOperand(&type7);
  auto param34 = model->addOperand(&type7);
  auto param35 = model->addOperand(&type7);
  auto param36 = model->addOperand(&type7);
  auto param37 = model->addOperand(&type8);
  auto param38 = model->addOperand(&type9);
  auto param39 = model->addOperand(&type9);
  auto param40 = model->addOperand(&type9);
  auto param41 = model->addOperand(&type7);
  auto param42 = model->addOperand(&type7);
  auto param43 = model->addOperand(&type8);
  auto scoresOut3 = model->addOperand(&type3);
  auto roiOut3 = model->addOperand(&type4);
  auto classesOut3 = model->addOperand(&type5);
  auto detectionOut3 = model->addOperand(&type6);
  // Phase 2, operations
  static float param33_init[] = {10.0f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static float param34_init[] = {10.0f};
  model->setOperandValue(param34, param34_init, sizeof(float) * 1);
  static float param35_init[] = {5.0f};
  model->setOperandValue(param35, param35_init, sizeof(float) * 1);
  static float param36_init[] = {5.0f};
  model->setOperandValue(param36, param36_init, sizeof(float) * 1);
  static bool8 param37_init[] = {false};
  model->setOperandValue(param37, param37_init, sizeof(bool8) * 1);
  static int32_t param38_init[] = {3};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {1};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {1};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static float param41_init[] = {0.0f};
  model->setOperandValue(param41, param41_init, sizeof(float) * 1);
  static float param42_init[] = {0.5f};
  model->setOperandValue(param42, param42_init, sizeof(float) * 1);
  static bool8 param43_init[] = {true};
  model->setOperandValue(param43, param43_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DETECTION_POSTPROCESSING, {scores3, roi3, anchors3, param33, param34, param35, param36, param37, param38, param39, param40, param41, param42, param43}, {scoresOut3, roiOut3, classesOut3, detectionOut3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores3, roi3, anchors3},
    {scoresOut3, roiOut3, classesOut3, detectionOut3});
  assert(model->isValid());
}

inline bool is_ignored_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_3(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 6, 3});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 6, 7});
  OperandType type2(Type::TENSOR_FLOAT32, {6, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {1, 3});
  OperandType type4(Type::TENSOR_FLOAT32, {1, 3, 4});
  OperandType type5(Type::TENSOR_INT32, {1, 3});
  OperandType type6(Type::TENSOR_INT32, {1});
  OperandType type7(Type::FLOAT32, {});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores3 = model->addOperand(&type0);
  auto roi3 = model->addOperand(&type10);
  auto anchors3 = model->addOperand(&type2);
  auto param33 = model->addOperand(&type7);
  auto param34 = model->addOperand(&type7);
  auto param35 = model->addOperand(&type7);
  auto param36 = model->addOperand(&type7);
  auto param37 = model->addOperand(&type8);
  auto param38 = model->addOperand(&type9);
  auto param39 = model->addOperand(&type9);
  auto param40 = model->addOperand(&type9);
  auto param41 = model->addOperand(&type7);
  auto param42 = model->addOperand(&type7);
  auto param43 = model->addOperand(&type8);
  auto scoresOut3 = model->addOperand(&type3);
  auto roiOut3 = model->addOperand(&type4);
  auto classesOut3 = model->addOperand(&type5);
  auto detectionOut3 = model->addOperand(&type6);
  // Phase 2, operations
  static float param33_init[] = {10.0f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static float param34_init[] = {10.0f};
  model->setOperandValue(param34, param34_init, sizeof(float) * 1);
  static float param35_init[] = {5.0f};
  model->setOperandValue(param35, param35_init, sizeof(float) * 1);
  static float param36_init[] = {5.0f};
  model->setOperandValue(param36, param36_init, sizeof(float) * 1);
  static bool8 param37_init[] = {false};
  model->setOperandValue(param37, param37_init, sizeof(bool8) * 1);
  static int32_t param38_init[] = {3};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {1};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {1};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static float param41_init[] = {0.0f};
  model->setOperandValue(param41, param41_init, sizeof(float) * 1);
  static float param42_init[] = {0.5f};
  model->setOperandValue(param42, param42_init, sizeof(float) * 1);
  static bool8 param43_init[] = {true};
  model->setOperandValue(param43, param43_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DETECTION_POSTPROCESSING, {scores3, roi3, anchors3, param33, param34, param35, param36, param37, param38, param39, param40, param41, param42, param43}, {scoresOut3, roiOut3, classesOut3, detectionOut3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores3, roi3, anchors3},
    {scoresOut3, roiOut3, classesOut3, detectionOut3});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_3(Model *model) {
  OperandType type11(Type::TENSOR_FLOAT16, {6, 4});
  OperandType type12(Type::FLOAT16, {});
  OperandType type14(Type::TENSOR_FLOAT16, {1, 3, 4});
  OperandType type15(Type::TENSOR_FLOAT16, {1, 6, 3});
  OperandType type16(Type::TENSOR_FLOAT16, {1, 3});
  OperandType type23(Type::TENSOR_FLOAT16, {1, 6, 7});
  OperandType type5(Type::TENSOR_INT32, {1, 3});
  OperandType type6(Type::TENSOR_INT32, {1});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores3 = model->addOperand(&type15);
  auto roi3 = model->addOperand(&type23);
  auto anchors3 = model->addOperand(&type11);
  auto param33 = model->addOperand(&type12);
  auto param34 = model->addOperand(&type12);
  auto param35 = model->addOperand(&type12);
  auto param36 = model->addOperand(&type12);
  auto param37 = model->addOperand(&type8);
  auto param38 = model->addOperand(&type9);
  auto param39 = model->addOperand(&type9);
  auto param40 = model->addOperand(&type9);
  auto param41 = model->addOperand(&type12);
  auto param42 = model->addOperand(&type12);
  auto param43 = model->addOperand(&type8);
  auto scoresOut3 = model->addOperand(&type16);
  auto roiOut3 = model->addOperand(&type14);
  auto classesOut3 = model->addOperand(&type5);
  auto detectionOut3 = model->addOperand(&type6);
  // Phase 2, operations
  static _Float16 param33_init[] = {10.0f};
  model->setOperandValue(param33, param33_init, sizeof(_Float16) * 1);
  static _Float16 param34_init[] = {10.0f};
  model->setOperandValue(param34, param34_init, sizeof(_Float16) * 1);
  static _Float16 param35_init[] = {5.0f};
  model->setOperandValue(param35, param35_init, sizeof(_Float16) * 1);
  static _Float16 param36_init[] = {5.0f};
  model->setOperandValue(param36, param36_init, sizeof(_Float16) * 1);
  static bool8 param37_init[] = {false};
  model->setOperandValue(param37, param37_init, sizeof(bool8) * 1);
  static int32_t param38_init[] = {3};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {1};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {1};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static _Float16 param41_init[] = {0.0f};
  model->setOperandValue(param41, param41_init, sizeof(_Float16) * 1);
  static _Float16 param42_init[] = {0.5f};
  model->setOperandValue(param42, param42_init, sizeof(_Float16) * 1);
  static bool8 param43_init[] = {true};
  model->setOperandValue(param43, param43_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DETECTION_POSTPROCESSING, {scores3, roi3, anchors3, param33, param34, param35, param36, param37, param38, param39, param40, param41, param42, param43}, {scoresOut3, roiOut3, classesOut3, detectionOut3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores3, roi3, anchors3},
    {scoresOut3, roiOut3, classesOut3, detectionOut3});
  assert(model->isValid());
}

inline bool is_ignored_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_3(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 6, 3});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 6, 7});
  OperandType type17(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type18(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type19(Type::TENSOR_INT32, {0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {6, 4});
  OperandType type20(Type::TENSOR_INT32, {0});
  OperandType type7(Type::FLOAT32, {});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores3 = model->addOperand(&type0);
  auto roi3 = model->addOperand(&type10);
  auto anchors3 = model->addOperand(&type2);
  auto param33 = model->addOperand(&type7);
  auto param34 = model->addOperand(&type7);
  auto param35 = model->addOperand(&type7);
  auto param36 = model->addOperand(&type7);
  auto param37 = model->addOperand(&type8);
  auto param38 = model->addOperand(&type9);
  auto param39 = model->addOperand(&type9);
  auto param40 = model->addOperand(&type9);
  auto param41 = model->addOperand(&type7);
  auto param42 = model->addOperand(&type7);
  auto param43 = model->addOperand(&type8);
  auto scoresOut3 = model->addOperand(&type17);
  auto roiOut3 = model->addOperand(&type18);
  auto classesOut3 = model->addOperand(&type19);
  auto detectionOut3 = model->addOperand(&type20);
  // Phase 2, operations
  static float param33_init[] = {10.0f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static float param34_init[] = {10.0f};
  model->setOperandValue(param34, param34_init, sizeof(float) * 1);
  static float param35_init[] = {5.0f};
  model->setOperandValue(param35, param35_init, sizeof(float) * 1);
  static float param36_init[] = {5.0f};
  model->setOperandValue(param36, param36_init, sizeof(float) * 1);
  static bool8 param37_init[] = {false};
  model->setOperandValue(param37, param37_init, sizeof(bool8) * 1);
  static int32_t param38_init[] = {3};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {1};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {1};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static float param41_init[] = {0.0f};
  model->setOperandValue(param41, param41_init, sizeof(float) * 1);
  static float param42_init[] = {0.5f};
  model->setOperandValue(param42, param42_init, sizeof(float) * 1);
  static bool8 param43_init[] = {true};
  model->setOperandValue(param43, param43_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DETECTION_POSTPROCESSING, {scores3, roi3, anchors3, param33, param34, param35, param36, param37, param38, param39, param40, param41, param42, param43}, {scoresOut3, roiOut3, classesOut3, detectionOut3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores3, roi3, anchors3},
    {scoresOut3, roiOut3, classesOut3, detectionOut3});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_3(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 6, 3});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 6, 7});
  OperandType type17(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type18(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type19(Type::TENSOR_INT32, {0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {6, 4});
  OperandType type20(Type::TENSOR_INT32, {0});
  OperandType type7(Type::FLOAT32, {});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores3 = model->addOperand(&type0);
  auto roi3 = model->addOperand(&type10);
  auto anchors3 = model->addOperand(&type2);
  auto param33 = model->addOperand(&type7);
  auto param34 = model->addOperand(&type7);
  auto param35 = model->addOperand(&type7);
  auto param36 = model->addOperand(&type7);
  auto param37 = model->addOperand(&type8);
  auto param38 = model->addOperand(&type9);
  auto param39 = model->addOperand(&type9);
  auto param40 = model->addOperand(&type9);
  auto param41 = model->addOperand(&type7);
  auto param42 = model->addOperand(&type7);
  auto param43 = model->addOperand(&type8);
  auto scoresOut3 = model->addOperand(&type17);
  auto roiOut3 = model->addOperand(&type18);
  auto classesOut3 = model->addOperand(&type19);
  auto detectionOut3 = model->addOperand(&type20);
  // Phase 2, operations
  static float param33_init[] = {10.0f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static float param34_init[] = {10.0f};
  model->setOperandValue(param34, param34_init, sizeof(float) * 1);
  static float param35_init[] = {5.0f};
  model->setOperandValue(param35, param35_init, sizeof(float) * 1);
  static float param36_init[] = {5.0f};
  model->setOperandValue(param36, param36_init, sizeof(float) * 1);
  static bool8 param37_init[] = {false};
  model->setOperandValue(param37, param37_init, sizeof(bool8) * 1);
  static int32_t param38_init[] = {3};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {1};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {1};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static float param41_init[] = {0.0f};
  model->setOperandValue(param41, param41_init, sizeof(float) * 1);
  static float param42_init[] = {0.5f};
  model->setOperandValue(param42, param42_init, sizeof(float) * 1);
  static bool8 param43_init[] = {true};
  model->setOperandValue(param43, param43_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DETECTION_POSTPROCESSING, {scores3, roi3, anchors3, param33, param34, param35, param36, param37, param38, param39, param40, param41, param42, param43}, {scoresOut3, roiOut3, classesOut3, detectionOut3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores3, roi3, anchors3},
    {scoresOut3, roiOut3, classesOut3, detectionOut3});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_3(Model *model) {
  OperandType type11(Type::TENSOR_FLOAT16, {6, 4});
  OperandType type12(Type::FLOAT16, {});
  OperandType type15(Type::TENSOR_FLOAT16, {1, 6, 3});
  OperandType type19(Type::TENSOR_INT32, {0, 0});
  OperandType type20(Type::TENSOR_INT32, {0});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type22(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type23(Type::TENSOR_FLOAT16, {1, 6, 7});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto scores3 = model->addOperand(&type15);
  auto roi3 = model->addOperand(&type23);
  auto anchors3 = model->addOperand(&type11);
  auto param33 = model->addOperand(&type12);
  auto param34 = model->addOperand(&type12);
  auto param35 = model->addOperand(&type12);
  auto param36 = model->addOperand(&type12);
  auto param37 = model->addOperand(&type8);
  auto param38 = model->addOperand(&type9);
  auto param39 = model->addOperand(&type9);
  auto param40 = model->addOperand(&type9);
  auto param41 = model->addOperand(&type12);
  auto param42 = model->addOperand(&type12);
  auto param43 = model->addOperand(&type8);
  auto scoresOut3 = model->addOperand(&type21);
  auto roiOut3 = model->addOperand(&type22);
  auto classesOut3 = model->addOperand(&type19);
  auto detectionOut3 = model->addOperand(&type20);
  // Phase 2, operations
  static _Float16 param33_init[] = {10.0f};
  model->setOperandValue(param33, param33_init, sizeof(_Float16) * 1);
  static _Float16 param34_init[] = {10.0f};
  model->setOperandValue(param34, param34_init, sizeof(_Float16) * 1);
  static _Float16 param35_init[] = {5.0f};
  model->setOperandValue(param35, param35_init, sizeof(_Float16) * 1);
  static _Float16 param36_init[] = {5.0f};
  model->setOperandValue(param36, param36_init, sizeof(_Float16) * 1);
  static bool8 param37_init[] = {false};
  model->setOperandValue(param37, param37_init, sizeof(bool8) * 1);
  static int32_t param38_init[] = {3};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {1};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {1};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static _Float16 param41_init[] = {0.0f};
  model->setOperandValue(param41, param41_init, sizeof(_Float16) * 1);
  static _Float16 param42_init[] = {0.5f};
  model->setOperandValue(param42, param42_init, sizeof(_Float16) * 1);
  static bool8 param43_init[] = {true};
  model->setOperandValue(param43, param43_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DETECTION_POSTPROCESSING, {scores3, roi3, anchors3, param33, param34, param35, param36, param37, param38, param39, param40, param41, param42, param43}, {scoresOut3, roiOut3, classesOut3, detectionOut3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores3, roi3, anchors3},
    {scoresOut3, roiOut3, classesOut3, detectionOut3});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

