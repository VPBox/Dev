// clang-format off
// Generated file (from: box_with_nms_limit_linear.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {19, 3});
  OperandType type1(Type::TENSOR_FLOAT32, {19, 12});
  OperandType type2(Type::TENSOR_INT32, {19});
  OperandType type3(Type::TENSOR_FLOAT32, {16});
  OperandType type4(Type::TENSOR_FLOAT32, {16, 4});
  OperandType type5(Type::TENSOR_INT32, {16});
  OperandType type6(Type::FLOAT32, {});
  OperandType type7(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type0);
  auto roi = model->addOperand(&type1);
  auto batchSplit = model->addOperand(&type2);
  auto param = model->addOperand(&type6);
  auto param1 = model->addOperand(&type7);
  auto param2 = model->addOperand(&type7);
  auto param3 = model->addOperand(&type6);
  auto param4 = model->addOperand(&type6);
  auto param5 = model->addOperand(&type6);
  auto scoresOut = model->addOperand(&type3);
  auto roiOut = model->addOperand(&type4);
  auto classesOut = model->addOperand(&type5);
  auto batchSplitOut = model->addOperand(&type5);
  // Phase 2, operations
  static float param_init[] = {0.3f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  static int32_t param1_init[] = {-1};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static float param3_init[] = {0.4f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static float param4_init[] = {1.0f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {0.3f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, batchSplit, param, param1, param2, param3, param4, param5}, {scoresOut, roiOut, classesOut, batchSplitOut});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores, roi, batchSplit},
    {scoresOut, roiOut, classesOut, batchSplitOut});
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {19, 3});
  OperandType type1(Type::TENSOR_FLOAT32, {19, 12});
  OperandType type2(Type::TENSOR_INT32, {19});
  OperandType type3(Type::TENSOR_FLOAT32, {16});
  OperandType type4(Type::TENSOR_FLOAT32, {16, 4});
  OperandType type5(Type::TENSOR_INT32, {16});
  OperandType type6(Type::FLOAT32, {});
  OperandType type7(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type0);
  auto roi = model->addOperand(&type1);
  auto batchSplit = model->addOperand(&type2);
  auto param = model->addOperand(&type6);
  auto param1 = model->addOperand(&type7);
  auto param2 = model->addOperand(&type7);
  auto param3 = model->addOperand(&type6);
  auto param4 = model->addOperand(&type6);
  auto param5 = model->addOperand(&type6);
  auto scoresOut = model->addOperand(&type3);
  auto roiOut = model->addOperand(&type4);
  auto classesOut = model->addOperand(&type5);
  auto batchSplitOut = model->addOperand(&type5);
  // Phase 2, operations
  static float param_init[] = {0.3f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  static int32_t param1_init[] = {-1};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static float param3_init[] = {0.4f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static float param4_init[] = {1.0f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {0.3f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, batchSplit, param, param1, param2, param3, param4, param5}, {scoresOut, roiOut, classesOut, batchSplitOut});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores, roi, batchSplit},
    {scoresOut, roiOut, classesOut, batchSplitOut});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16(Model *model) {
  OperandType type11(Type::FLOAT16, {});
  OperandType type12(Type::TENSOR_FLOAT16, {19, 12});
  OperandType type13(Type::TENSOR_FLOAT16, {16, 4});
  OperandType type14(Type::TENSOR_FLOAT16, {19, 3});
  OperandType type15(Type::TENSOR_FLOAT16, {16});
  OperandType type2(Type::TENSOR_INT32, {19});
  OperandType type5(Type::TENSOR_INT32, {16});
  OperandType type7(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type14);
  auto roi = model->addOperand(&type12);
  auto batchSplit = model->addOperand(&type2);
  auto param = model->addOperand(&type11);
  auto param1 = model->addOperand(&type7);
  auto param2 = model->addOperand(&type7);
  auto param3 = model->addOperand(&type11);
  auto param4 = model->addOperand(&type11);
  auto param5 = model->addOperand(&type11);
  auto scoresOut = model->addOperand(&type15);
  auto roiOut = model->addOperand(&type13);
  auto classesOut = model->addOperand(&type5);
  auto batchSplitOut = model->addOperand(&type5);
  // Phase 2, operations
  static _Float16 param_init[] = {0.30000001192092896f};
  model->setOperandValue(param, param_init, sizeof(_Float16) * 1);
  static int32_t param1_init[] = {-1};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static _Float16 param3_init[] = {0.4000000059604645f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static _Float16 param4_init[] = {1.0f};
  model->setOperandValue(param4, param4_init, sizeof(_Float16) * 1);
  static _Float16 param5_init[] = {0.30000001192092896f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, batchSplit, param, param1, param2, param3, param4, param5}, {scoresOut, roiOut, classesOut, batchSplitOut});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores, roi, batchSplit},
    {scoresOut, roiOut, classesOut, batchSplitOut});
  assert(model->isValid());
}

inline bool is_ignored_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant8(Model *model) {
  OperandType type16(Type::TENSOR_QUANT16_ASYMM, {19, 12}, 0.125f, 0);
  OperandType type17(Type::TENSOR_QUANT16_ASYMM, {16, 4}, 0.125f, 0);
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {19, 3}, 0.01f, 0);
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {16}, 0.01f, 0);
  OperandType type2(Type::TENSOR_INT32, {19});
  OperandType type5(Type::TENSOR_INT32, {16});
  OperandType type6(Type::FLOAT32, {});
  OperandType type7(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type18);
  auto roi = model->addOperand(&type16);
  auto batchSplit = model->addOperand(&type2);
  auto param = model->addOperand(&type6);
  auto param1 = model->addOperand(&type7);
  auto param2 = model->addOperand(&type7);
  auto param3 = model->addOperand(&type6);
  auto param4 = model->addOperand(&type6);
  auto param5 = model->addOperand(&type6);
  auto scoresOut = model->addOperand(&type19);
  auto roiOut = model->addOperand(&type17);
  auto classesOut = model->addOperand(&type5);
  auto batchSplitOut = model->addOperand(&type5);
  // Phase 2, operations
  static float param_init[] = {0.3f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  static int32_t param1_init[] = {-1};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static float param3_init[] = {0.4f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static float param4_init[] = {1.0f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {0.3f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, batchSplit, param, param1, param2, param3, param4, param5}, {scoresOut, roiOut, classesOut, batchSplitOut});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores, roi, batchSplit},
    {scoresOut, roiOut, classesOut, batchSplitOut});
  assert(model->isValid());
}

inline bool is_ignored_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {19, 3});
  OperandType type1(Type::TENSOR_FLOAT32, {19, 12});
  OperandType type2(Type::TENSOR_INT32, {19});
  OperandType type20(Type::TENSOR_FLOAT32, {0});
  OperandType type21(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type22(Type::TENSOR_INT32, {0});
  OperandType type6(Type::FLOAT32, {});
  OperandType type7(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type0);
  auto roi = model->addOperand(&type1);
  auto batchSplit = model->addOperand(&type2);
  auto param = model->addOperand(&type6);
  auto param1 = model->addOperand(&type7);
  auto param2 = model->addOperand(&type7);
  auto param3 = model->addOperand(&type6);
  auto param4 = model->addOperand(&type6);
  auto param5 = model->addOperand(&type6);
  auto scoresOut = model->addOperand(&type20);
  auto roiOut = model->addOperand(&type21);
  auto classesOut = model->addOperand(&type22);
  auto batchSplitOut = model->addOperand(&type22);
  // Phase 2, operations
  static float param_init[] = {0.3f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  static int32_t param1_init[] = {-1};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static float param3_init[] = {0.4f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static float param4_init[] = {1.0f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {0.3f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, batchSplit, param, param1, param2, param3, param4, param5}, {scoresOut, roiOut, classesOut, batchSplitOut});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores, roi, batchSplit},
    {scoresOut, roiOut, classesOut, batchSplitOut});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {19, 3});
  OperandType type1(Type::TENSOR_FLOAT32, {19, 12});
  OperandType type2(Type::TENSOR_INT32, {19});
  OperandType type20(Type::TENSOR_FLOAT32, {0});
  OperandType type21(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type22(Type::TENSOR_INT32, {0});
  OperandType type6(Type::FLOAT32, {});
  OperandType type7(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type0);
  auto roi = model->addOperand(&type1);
  auto batchSplit = model->addOperand(&type2);
  auto param = model->addOperand(&type6);
  auto param1 = model->addOperand(&type7);
  auto param2 = model->addOperand(&type7);
  auto param3 = model->addOperand(&type6);
  auto param4 = model->addOperand(&type6);
  auto param5 = model->addOperand(&type6);
  auto scoresOut = model->addOperand(&type20);
  auto roiOut = model->addOperand(&type21);
  auto classesOut = model->addOperand(&type22);
  auto batchSplitOut = model->addOperand(&type22);
  // Phase 2, operations
  static float param_init[] = {0.3f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  static int32_t param1_init[] = {-1};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static float param3_init[] = {0.4f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static float param4_init[] = {1.0f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {0.3f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, batchSplit, param, param1, param2, param3, param4, param5}, {scoresOut, roiOut, classesOut, batchSplitOut});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores, roi, batchSplit},
    {scoresOut, roiOut, classesOut, batchSplitOut});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16(Model *model) {
  OperandType type11(Type::FLOAT16, {});
  OperandType type12(Type::TENSOR_FLOAT16, {19, 12});
  OperandType type14(Type::TENSOR_FLOAT16, {19, 3});
  OperandType type2(Type::TENSOR_INT32, {19});
  OperandType type22(Type::TENSOR_INT32, {0});
  OperandType type23(Type::TENSOR_FLOAT16, {0});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type7(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type14);
  auto roi = model->addOperand(&type12);
  auto batchSplit = model->addOperand(&type2);
  auto param = model->addOperand(&type11);
  auto param1 = model->addOperand(&type7);
  auto param2 = model->addOperand(&type7);
  auto param3 = model->addOperand(&type11);
  auto param4 = model->addOperand(&type11);
  auto param5 = model->addOperand(&type11);
  auto scoresOut = model->addOperand(&type23);
  auto roiOut = model->addOperand(&type24);
  auto classesOut = model->addOperand(&type22);
  auto batchSplitOut = model->addOperand(&type22);
  // Phase 2, operations
  static _Float16 param_init[] = {0.30000001192092896f};
  model->setOperandValue(param, param_init, sizeof(_Float16) * 1);
  static int32_t param1_init[] = {-1};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static _Float16 param3_init[] = {0.4000000059604645f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static _Float16 param4_init[] = {1.0f};
  model->setOperandValue(param4, param4_init, sizeof(_Float16) * 1);
  static _Float16 param5_init[] = {0.30000001192092896f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, batchSplit, param, param1, param2, param3, param4, param5}, {scoresOut, roiOut, classesOut, batchSplitOut});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores, roi, batchSplit},
    {scoresOut, roiOut, classesOut, batchSplitOut});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_quant8(Model *model) {
  OperandType type16(Type::TENSOR_QUANT16_ASYMM, {19, 12}, 0.125f, 0);
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {19, 3}, 0.01f, 0);
  OperandType type2(Type::TENSOR_INT32, {19});
  OperandType type22(Type::TENSOR_INT32, {0});
  OperandType type25(Type::TENSOR_QUANT8_ASYMM, {0}, 0.01f, 0);
  OperandType type26(Type::TENSOR_QUANT16_ASYMM, {0, 0}, 0.125f, 0);
  OperandType type6(Type::FLOAT32, {});
  OperandType type7(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type18);
  auto roi = model->addOperand(&type16);
  auto batchSplit = model->addOperand(&type2);
  auto param = model->addOperand(&type6);
  auto param1 = model->addOperand(&type7);
  auto param2 = model->addOperand(&type7);
  auto param3 = model->addOperand(&type6);
  auto param4 = model->addOperand(&type6);
  auto param5 = model->addOperand(&type6);
  auto scoresOut = model->addOperand(&type25);
  auto roiOut = model->addOperand(&type26);
  auto classesOut = model->addOperand(&type22);
  auto batchSplitOut = model->addOperand(&type22);
  // Phase 2, operations
  static float param_init[] = {0.3f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  static int32_t param1_init[] = {-1};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static float param3_init[] = {0.4f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static float param4_init[] = {1.0f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {0.3f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, batchSplit, param, param1, param2, param3, param4, param5}, {scoresOut, roiOut, classesOut, batchSplitOut});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores, roi, batchSplit},
    {scoresOut, roiOut, classesOut, batchSplitOut});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {19, 3});
  OperandType type1(Type::TENSOR_FLOAT32, {19, 12});
  OperandType type10(Type::TENSOR_INT32, {15});
  OperandType type2(Type::TENSOR_INT32, {19});
  OperandType type6(Type::FLOAT32, {});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {15});
  OperandType type9(Type::TENSOR_FLOAT32, {15, 4});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type0);
  auto roi1 = model->addOperand(&type1);
  auto batchSplit1 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type6);
  auto param7 = model->addOperand(&type7);
  auto param8 = model->addOperand(&type7);
  auto param9 = model->addOperand(&type6);
  auto param10 = model->addOperand(&type6);
  auto param11 = model->addOperand(&type6);
  auto scoresOut1 = model->addOperand(&type8);
  auto roiOut1 = model->addOperand(&type9);
  auto classesOut1 = model->addOperand(&type10);
  auto batchSplitOut1 = model->addOperand(&type10);
  // Phase 2, operations
  static float param6_init[] = {0.3f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static int32_t param7_init[] = {8};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {0.4f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {0.5f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.3f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, batchSplit1, param6, param7, param8, param9, param10, param11}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores1, roi1, batchSplit1},
    {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  assert(model->isValid());
}

inline bool is_ignored_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {19, 3});
  OperandType type1(Type::TENSOR_FLOAT32, {19, 12});
  OperandType type10(Type::TENSOR_INT32, {15});
  OperandType type2(Type::TENSOR_INT32, {19});
  OperandType type6(Type::FLOAT32, {});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {15});
  OperandType type9(Type::TENSOR_FLOAT32, {15, 4});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type0);
  auto roi1 = model->addOperand(&type1);
  auto batchSplit1 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type6);
  auto param7 = model->addOperand(&type7);
  auto param8 = model->addOperand(&type7);
  auto param9 = model->addOperand(&type6);
  auto param10 = model->addOperand(&type6);
  auto param11 = model->addOperand(&type6);
  auto scoresOut1 = model->addOperand(&type8);
  auto roiOut1 = model->addOperand(&type9);
  auto classesOut1 = model->addOperand(&type10);
  auto batchSplitOut1 = model->addOperand(&type10);
  // Phase 2, operations
  static float param6_init[] = {0.3f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static int32_t param7_init[] = {8};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {0.4f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {0.5f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.3f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, batchSplit1, param6, param7, param8, param9, param10, param11}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores1, roi1, batchSplit1},
    {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_2(Model *model) {
  OperandType type10(Type::TENSOR_INT32, {15});
  OperandType type11(Type::FLOAT16, {});
  OperandType type12(Type::TENSOR_FLOAT16, {19, 12});
  OperandType type14(Type::TENSOR_FLOAT16, {19, 3});
  OperandType type2(Type::TENSOR_INT32, {19});
  OperandType type27(Type::TENSOR_FLOAT16, {15, 4});
  OperandType type28(Type::TENSOR_FLOAT16, {15});
  OperandType type7(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type14);
  auto roi1 = model->addOperand(&type12);
  auto batchSplit1 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type11);
  auto param7 = model->addOperand(&type7);
  auto param8 = model->addOperand(&type7);
  auto param9 = model->addOperand(&type11);
  auto param10 = model->addOperand(&type11);
  auto param11 = model->addOperand(&type11);
  auto scoresOut1 = model->addOperand(&type28);
  auto roiOut1 = model->addOperand(&type27);
  auto classesOut1 = model->addOperand(&type10);
  auto batchSplitOut1 = model->addOperand(&type10);
  // Phase 2, operations
  static _Float16 param6_init[] = {0.30000001192092896f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static int32_t param7_init[] = {8};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {0.4000000059604645f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {0.5f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.30000001192092896f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, batchSplit1, param6, param7, param8, param9, param10, param11}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores1, roi1, batchSplit1},
    {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  assert(model->isValid());
}

inline bool is_ignored_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant8_2(Model *model) {
  OperandType type10(Type::TENSOR_INT32, {15});
  OperandType type16(Type::TENSOR_QUANT16_ASYMM, {19, 12}, 0.125f, 0);
  OperandType type2(Type::TENSOR_INT32, {19});
  OperandType type29(Type::TENSOR_QUANT16_ASYMM, {15, 4}, 0.125f, 0);
  OperandType type30(Type::TENSOR_QUANT8_ASYMM, {19, 3}, 0.01f, 128);
  OperandType type31(Type::TENSOR_QUANT8_ASYMM, {15}, 0.01f, 128);
  OperandType type6(Type::FLOAT32, {});
  OperandType type7(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type30);
  auto roi1 = model->addOperand(&type16);
  auto batchSplit1 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type6);
  auto param7 = model->addOperand(&type7);
  auto param8 = model->addOperand(&type7);
  auto param9 = model->addOperand(&type6);
  auto param10 = model->addOperand(&type6);
  auto param11 = model->addOperand(&type6);
  auto scoresOut1 = model->addOperand(&type31);
  auto roiOut1 = model->addOperand(&type29);
  auto classesOut1 = model->addOperand(&type10);
  auto batchSplitOut1 = model->addOperand(&type10);
  // Phase 2, operations
  static float param6_init[] = {0.3f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static int32_t param7_init[] = {8};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {0.4f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {0.5f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.3f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, batchSplit1, param6, param7, param8, param9, param10, param11}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores1, roi1, batchSplit1},
    {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  assert(model->isValid());
}

inline bool is_ignored_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {19, 3});
  OperandType type1(Type::TENSOR_FLOAT32, {19, 12});
  OperandType type2(Type::TENSOR_INT32, {19});
  OperandType type20(Type::TENSOR_FLOAT32, {0});
  OperandType type21(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type22(Type::TENSOR_INT32, {0});
  OperandType type6(Type::FLOAT32, {});
  OperandType type7(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type0);
  auto roi1 = model->addOperand(&type1);
  auto batchSplit1 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type6);
  auto param7 = model->addOperand(&type7);
  auto param8 = model->addOperand(&type7);
  auto param9 = model->addOperand(&type6);
  auto param10 = model->addOperand(&type6);
  auto param11 = model->addOperand(&type6);
  auto scoresOut1 = model->addOperand(&type20);
  auto roiOut1 = model->addOperand(&type21);
  auto classesOut1 = model->addOperand(&type22);
  auto batchSplitOut1 = model->addOperand(&type22);
  // Phase 2, operations
  static float param6_init[] = {0.3f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static int32_t param7_init[] = {8};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {0.4f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {0.5f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.3f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, batchSplit1, param6, param7, param8, param9, param10, param11}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores1, roi1, batchSplit1},
    {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {19, 3});
  OperandType type1(Type::TENSOR_FLOAT32, {19, 12});
  OperandType type2(Type::TENSOR_INT32, {19});
  OperandType type20(Type::TENSOR_FLOAT32, {0});
  OperandType type21(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type22(Type::TENSOR_INT32, {0});
  OperandType type6(Type::FLOAT32, {});
  OperandType type7(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type0);
  auto roi1 = model->addOperand(&type1);
  auto batchSplit1 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type6);
  auto param7 = model->addOperand(&type7);
  auto param8 = model->addOperand(&type7);
  auto param9 = model->addOperand(&type6);
  auto param10 = model->addOperand(&type6);
  auto param11 = model->addOperand(&type6);
  auto scoresOut1 = model->addOperand(&type20);
  auto roiOut1 = model->addOperand(&type21);
  auto classesOut1 = model->addOperand(&type22);
  auto batchSplitOut1 = model->addOperand(&type22);
  // Phase 2, operations
  static float param6_init[] = {0.3f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static int32_t param7_init[] = {8};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {0.4f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {0.5f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.3f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, batchSplit1, param6, param7, param8, param9, param10, param11}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores1, roi1, batchSplit1},
    {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_2(Model *model) {
  OperandType type11(Type::FLOAT16, {});
  OperandType type12(Type::TENSOR_FLOAT16, {19, 12});
  OperandType type14(Type::TENSOR_FLOAT16, {19, 3});
  OperandType type2(Type::TENSOR_INT32, {19});
  OperandType type22(Type::TENSOR_INT32, {0});
  OperandType type23(Type::TENSOR_FLOAT16, {0});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type7(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type14);
  auto roi1 = model->addOperand(&type12);
  auto batchSplit1 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type11);
  auto param7 = model->addOperand(&type7);
  auto param8 = model->addOperand(&type7);
  auto param9 = model->addOperand(&type11);
  auto param10 = model->addOperand(&type11);
  auto param11 = model->addOperand(&type11);
  auto scoresOut1 = model->addOperand(&type23);
  auto roiOut1 = model->addOperand(&type24);
  auto classesOut1 = model->addOperand(&type22);
  auto batchSplitOut1 = model->addOperand(&type22);
  // Phase 2, operations
  static _Float16 param6_init[] = {0.30000001192092896f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static int32_t param7_init[] = {8};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {0.4000000059604645f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {0.5f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.30000001192092896f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, batchSplit1, param6, param7, param8, param9, param10, param11}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores1, roi1, batchSplit1},
    {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_quant8_2(Model *model) {
  OperandType type16(Type::TENSOR_QUANT16_ASYMM, {19, 12}, 0.125f, 0);
  OperandType type2(Type::TENSOR_INT32, {19});
  OperandType type22(Type::TENSOR_INT32, {0});
  OperandType type26(Type::TENSOR_QUANT16_ASYMM, {0, 0}, 0.125f, 0);
  OperandType type30(Type::TENSOR_QUANT8_ASYMM, {19, 3}, 0.01f, 128);
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {0}, 0.01f, 128);
  OperandType type6(Type::FLOAT32, {});
  OperandType type7(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type30);
  auto roi1 = model->addOperand(&type16);
  auto batchSplit1 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type6);
  auto param7 = model->addOperand(&type7);
  auto param8 = model->addOperand(&type7);
  auto param9 = model->addOperand(&type6);
  auto param10 = model->addOperand(&type6);
  auto param11 = model->addOperand(&type6);
  auto scoresOut1 = model->addOperand(&type32);
  auto roiOut1 = model->addOperand(&type26);
  auto classesOut1 = model->addOperand(&type22);
  auto batchSplitOut1 = model->addOperand(&type22);
  // Phase 2, operations
  static float param6_init[] = {0.3f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static int32_t param7_init[] = {8};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {0.4f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {0.5f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.3f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, batchSplit1, param6, param7, param8, param9, param10, param11}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {scores1, roi1, batchSplit1},
    {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

