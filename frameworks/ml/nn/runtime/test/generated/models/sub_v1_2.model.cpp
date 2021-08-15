// clang-format off
// Generated file (from: sub_v1_2.mod.py). Do not edit
void CreateModel_none(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type1(Type::INT32, {});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto input1 = model->addOperand(&type0);
  auto act = model->addOperand(&type1);
  auto output0 = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input0, input1, act}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_none(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relu(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type1(Type::INT32, {});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto input1 = model->addOperand(&type0);
  auto act = model->addOperand(&type1);
  auto output0 = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input0, input1, act}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_relu(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relu1(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type1(Type::INT32, {});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto input1 = model->addOperand(&type0);
  auto act = model->addOperand(&type1);
  auto output0 = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input0, input1, act}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_relu1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relu6(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type1(Type::INT32, {});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto input1 = model->addOperand(&type0);
  auto act = model->addOperand(&type1);
  auto output0 = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input0, input1, act}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_relu6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_none(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  // Phase 1, operands
  auto input0 = model->addOperand(&type13);
  auto input1 = model->addOperand(&type13);
  auto act = model->addOperand(&type1);
  auto output0 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input0, input1, act}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_float16_none(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_relu(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  // Phase 1, operands
  auto input0 = model->addOperand(&type13);
  auto input1 = model->addOperand(&type13);
  auto act = model->addOperand(&type1);
  auto output0 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input0, input1, act}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_float16_relu(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_relu1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  // Phase 1, operands
  auto input0 = model->addOperand(&type13);
  auto input1 = model->addOperand(&type13);
  auto act = model->addOperand(&type1);
  auto output0 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input0, input1, act}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_float16_relu1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_relu6(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  // Phase 1, operands
  auto input0 = model->addOperand(&type13);
  auto input1 = model->addOperand(&type13);
  auto act = model->addOperand(&type1);
  auto output0 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input0, input1, act}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_float16_relu6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_none(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type1(Type::INT32, {});
  OperandType type14(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto input1 = model->addOperand(&type0);
  auto act = model->addOperand(&type1);
  auto output0 = model->addOperand(&type14);
  // Phase 2, operations
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input0, input1, act}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_none(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relu(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type1(Type::INT32, {});
  OperandType type14(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto input1 = model->addOperand(&type0);
  auto act = model->addOperand(&type1);
  auto output0 = model->addOperand(&type14);
  // Phase 2, operations
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input0, input1, act}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relu(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relu1(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type1(Type::INT32, {});
  OperandType type14(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto input1 = model->addOperand(&type0);
  auto act = model->addOperand(&type1);
  auto output0 = model->addOperand(&type14);
  // Phase 2, operations
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input0, input1, act}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relu1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relu6(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type1(Type::INT32, {});
  OperandType type14(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto input1 = model->addOperand(&type0);
  auto act = model->addOperand(&type1);
  auto output0 = model->addOperand(&type14);
  // Phase 2, operations
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input0, input1, act}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relu6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_none(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type15(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type13);
  auto input1 = model->addOperand(&type13);
  auto act = model->addOperand(&type1);
  auto output0 = model->addOperand(&type15);
  // Phase 2, operations
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input0, input1, act}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_none(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_relu(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type15(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type13);
  auto input1 = model->addOperand(&type13);
  auto act = model->addOperand(&type1);
  auto output0 = model->addOperand(&type15);
  // Phase 2, operations
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input0, input1, act}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_relu(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_relu1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type15(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type13);
  auto input1 = model->addOperand(&type13);
  auto act = model->addOperand(&type1);
  auto output0 = model->addOperand(&type15);
  // Phase 2, operations
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input0, input1, act}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_relu1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_relu6(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type15(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type13);
  auto input1 = model->addOperand(&type13);
  auto act = model->addOperand(&type1);
  auto output0 = model->addOperand(&type15);
  // Phase 2, operations
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input0, input1, act}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_relu6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant8(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {2, 4, 16, 2}, 0.5f, 0);
  // Phase 1, operands
  auto input01 = model->addOperand(&type2);
  auto input11 = model->addOperand(&type2);
  auto param = model->addOperand(&type1);
  auto output01 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input01, input11, param}, {output01});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input01, input11},
    {output01});
  assert(model->isValid());
}

inline bool is_ignored_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant8_dynamic_output_shape(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type16(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 0);
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {2, 4, 16, 2}, 0.5f, 0);
  // Phase 1, operands
  auto input01 = model->addOperand(&type2);
  auto input11 = model->addOperand(&type2);
  auto param = model->addOperand(&type1);
  auto output01 = model->addOperand(&type16);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input01, input11, param}, {output01});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input01, input11},
    {output01});
  assert(model->isValid());
}

inline bool is_ignored_quant8_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 1, 1, 2});
  OperandType type12(Type::TENSOR_FLOAT32, {0, 2, 2, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type4(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type6(Type::TENSOR_INT32, {0});
  OperandType type7(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type8(Type::TENSOR_INT32, {1});
  OperandType type9(Type::FLOAT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type3);
  auto roi = model->addOperand(&type4);
  auto param1 = model->addOperand(&type8);
  auto param2 = model->addOperand(&type9);
  auto param3 = model->addOperand(&type1);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type9);
  auto param6 = model->addOperand(&type9);
  auto param7 = model->addOperand(&type9);
  auto scoresOut = model->addOperand(&type5);
  auto roiOut = model->addOperand(&type7);
  auto classesOut = model->addOperand(&type6);
  auto batchSplitOut = model->addOperand(&type6);
  auto in = model->addOperand(&type11);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type1);
  auto param10 = model->addOperand(&type9);
  auto param11 = model->addOperand(&type9);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type1);
  auto layout = model->addOperand(&type10);
  auto featureMap = model->addOperand(&type12);
  auto op = model->addOperand(&type0);
  auto param14 = model->addOperand(&type1);
  auto out = model->addOperand(&type12);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static float param2_init[] = {0.3f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t param3_init[] = {-1};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {0};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {0.4f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {1.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.3f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {2};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static float param10_init[] = {2.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {2.0f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t param12_init[] = {4};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {4};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float op_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op, op_init, sizeof(float) * 4);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param1, param2, param3, param4, param5, param6, param7}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param8, param9, param10, param11, param12, param13, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_SUB, {featureMap, op, param14}, {out});
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
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 1, 1, 2});
  OperandType type12(Type::TENSOR_FLOAT32, {0, 2, 2, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type4(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type6(Type::TENSOR_INT32, {0});
  OperandType type7(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type8(Type::TENSOR_INT32, {1});
  OperandType type9(Type::FLOAT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type3);
  auto roi = model->addOperand(&type4);
  auto param1 = model->addOperand(&type8);
  auto param2 = model->addOperand(&type9);
  auto param3 = model->addOperand(&type1);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type9);
  auto param6 = model->addOperand(&type9);
  auto param7 = model->addOperand(&type9);
  auto scoresOut = model->addOperand(&type5);
  auto roiOut = model->addOperand(&type7);
  auto classesOut = model->addOperand(&type6);
  auto batchSplitOut = model->addOperand(&type6);
  auto in = model->addOperand(&type11);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type1);
  auto param10 = model->addOperand(&type9);
  auto param11 = model->addOperand(&type9);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type1);
  auto layout = model->addOperand(&type10);
  auto featureMap = model->addOperand(&type12);
  auto op = model->addOperand(&type0);
  auto param14 = model->addOperand(&type1);
  auto out = model->addOperand(&type12);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static float param2_init[] = {0.3f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t param3_init[] = {-1};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {0};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {0.4f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {1.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.3f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {2};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static float param10_init[] = {2.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {2.0f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t param12_init[] = {4};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {4};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float op_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op, op_init, sizeof(float) * 4);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param1, param2, param3, param4, param5, param6, param7}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param8, param9, param10, param11, param12, param13, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_SUB, {featureMap, op, param14}, {out});
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
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::BOOL, {});
  OperandType type17(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 2}, 0.1f, 128);
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 2}, 0.1f, 128);
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.1f, 128);
  OperandType type20(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type21(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type22(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  OperandType type6(Type::TENSOR_INT32, {0});
  OperandType type8(Type::TENSOR_INT32, {1});
  OperandType type9(Type::FLOAT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type22);
  auto roi = model->addOperand(&type20);
  auto param1 = model->addOperand(&type8);
  auto param2 = model->addOperand(&type9);
  auto param3 = model->addOperand(&type1);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type9);
  auto param6 = model->addOperand(&type9);
  auto param7 = model->addOperand(&type9);
  auto scoresOut = model->addOperand(&type23);
  auto roiOut = model->addOperand(&type21);
  auto classesOut = model->addOperand(&type6);
  auto batchSplitOut = model->addOperand(&type6);
  auto in = model->addOperand(&type18);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type1);
  auto param10 = model->addOperand(&type9);
  auto param11 = model->addOperand(&type9);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type1);
  auto layout = model->addOperand(&type10);
  auto featureMap = model->addOperand(&type17);
  auto op = model->addOperand(&type19);
  auto param14 = model->addOperand(&type1);
  auto out = model->addOperand(&type17);
  // Phase 2, operations
  static uint8_t scores_init[] = {137, 129};
  model->setOperandValue(scores, scores_init, sizeof(uint8_t) * 2);
  static uint16_t roi_init[] = {8, 8, 80, 80, 0, 0, 80, 80};
  model->setOperandValue(roi, roi_init, sizeof(uint16_t) * 8);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static float param2_init[] = {0.3f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t param3_init[] = {-1};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {0};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {0.4f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {1.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.3f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {2};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static float param10_init[] = {2.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {2.0f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t param12_init[] = {4};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {4};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static uint8_t op_init[] = {138, 148, 158, 168};
  model->setOperandValue(op, op_init, sizeof(uint8_t) * 4);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param1, param2, param3, param4, param5, param6, param7}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param8, param9, param10, param11, param12, param13, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_SUB, {featureMap, op, param14}, {out});
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
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::BOOL, {});
  OperandType type13(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 2, 2, 2});
  OperandType type25(Type::TENSOR_FLOAT16, {1, 1, 1, 2});
  OperandType type26(Type::FLOAT16, {});
  OperandType type27(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type29(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type30(Type::TENSOR_FLOAT16, {0});
  OperandType type6(Type::TENSOR_INT32, {0});
  OperandType type8(Type::TENSOR_INT32, {1});
  // Phase 1, operands
  auto scores = model->addOperand(&type29);
  auto roi = model->addOperand(&type27);
  auto param1 = model->addOperand(&type8);
  auto param2 = model->addOperand(&type26);
  auto param3 = model->addOperand(&type1);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type26);
  auto param6 = model->addOperand(&type26);
  auto param7 = model->addOperand(&type26);
  auto scoresOut = model->addOperand(&type30);
  auto roiOut = model->addOperand(&type28);
  auto classesOut = model->addOperand(&type6);
  auto batchSplitOut = model->addOperand(&type6);
  auto in = model->addOperand(&type25);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type1);
  auto param10 = model->addOperand(&type26);
  auto param11 = model->addOperand(&type26);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type1);
  auto layout = model->addOperand(&type10);
  auto featureMap = model->addOperand(&type24);
  auto op = model->addOperand(&type13);
  auto param14 = model->addOperand(&type1);
  auto out = model->addOperand(&type24);
  // Phase 2, operations
  static _Float16 scores_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores, scores_init, sizeof(_Float16) * 2);
  static _Float16 roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(_Float16) * 8);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static _Float16 param2_init[] = {0.30000001192092896f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t param3_init[] = {-1};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {0};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {0.4000000059604645f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {1.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.30000001192092896f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {2};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static _Float16 param10_init[] = {2.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {2.0f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t param12_init[] = {4};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {4};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static _Float16 op_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op, op_init, sizeof(_Float16) * 4);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param1, param2, param3, param4, param5, param6, param7}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param8, param9, param10, param11, param12, param13, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_SUB, {featureMap, op, param14}, {out});
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
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 1, 1, 2});
  OperandType type12(Type::TENSOR_FLOAT32, {0, 2, 2, 2});
  OperandType type14(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type4(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type6(Type::TENSOR_INT32, {0});
  OperandType type7(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type8(Type::TENSOR_INT32, {1});
  OperandType type9(Type::FLOAT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type3);
  auto roi = model->addOperand(&type4);
  auto param1 = model->addOperand(&type8);
  auto param2 = model->addOperand(&type9);
  auto param3 = model->addOperand(&type1);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type9);
  auto param6 = model->addOperand(&type9);
  auto param7 = model->addOperand(&type9);
  auto scoresOut = model->addOperand(&type5);
  auto roiOut = model->addOperand(&type7);
  auto classesOut = model->addOperand(&type6);
  auto batchSplitOut = model->addOperand(&type6);
  auto in = model->addOperand(&type11);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type1);
  auto param10 = model->addOperand(&type9);
  auto param11 = model->addOperand(&type9);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type1);
  auto layout = model->addOperand(&type10);
  auto featureMap = model->addOperand(&type12);
  auto op = model->addOperand(&type0);
  auto param14 = model->addOperand(&type1);
  auto out = model->addOperand(&type14);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static float param2_init[] = {0.3f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t param3_init[] = {-1};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {0};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {0.4f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {1.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.3f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {2};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static float param10_init[] = {2.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {2.0f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t param12_init[] = {4};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {4};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float op_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op, op_init, sizeof(float) * 4);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param1, param2, param3, param4, param5, param6, param7}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param8, param9, param10, param11, param12, param13, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_SUB, {featureMap, op, param14}, {out});
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
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 1, 1, 2});
  OperandType type12(Type::TENSOR_FLOAT32, {0, 2, 2, 2});
  OperandType type14(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type4(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type6(Type::TENSOR_INT32, {0});
  OperandType type7(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type8(Type::TENSOR_INT32, {1});
  OperandType type9(Type::FLOAT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type3);
  auto roi = model->addOperand(&type4);
  auto param1 = model->addOperand(&type8);
  auto param2 = model->addOperand(&type9);
  auto param3 = model->addOperand(&type1);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type9);
  auto param6 = model->addOperand(&type9);
  auto param7 = model->addOperand(&type9);
  auto scoresOut = model->addOperand(&type5);
  auto roiOut = model->addOperand(&type7);
  auto classesOut = model->addOperand(&type6);
  auto batchSplitOut = model->addOperand(&type6);
  auto in = model->addOperand(&type11);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type1);
  auto param10 = model->addOperand(&type9);
  auto param11 = model->addOperand(&type9);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type1);
  auto layout = model->addOperand(&type10);
  auto featureMap = model->addOperand(&type12);
  auto op = model->addOperand(&type0);
  auto param14 = model->addOperand(&type1);
  auto out = model->addOperand(&type14);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static float param2_init[] = {0.3f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t param3_init[] = {-1};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {0};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {0.4f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {1.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.3f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {2};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static float param10_init[] = {2.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {2.0f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t param12_init[] = {4};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {4};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float op_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op, op_init, sizeof(float) * 4);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param1, param2, param3, param4, param5, param6, param7}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param8, param9, param10, param11, param12, param13, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_SUB, {featureMap, op, param14}, {out});
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
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::BOOL, {});
  OperandType type17(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 2}, 0.1f, 128);
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 2}, 0.1f, 128);
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.1f, 128);
  OperandType type20(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type21(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type22(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  OperandType type31(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 128);
  OperandType type6(Type::TENSOR_INT32, {0});
  OperandType type8(Type::TENSOR_INT32, {1});
  OperandType type9(Type::FLOAT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type22);
  auto roi = model->addOperand(&type20);
  auto param1 = model->addOperand(&type8);
  auto param2 = model->addOperand(&type9);
  auto param3 = model->addOperand(&type1);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type9);
  auto param6 = model->addOperand(&type9);
  auto param7 = model->addOperand(&type9);
  auto scoresOut = model->addOperand(&type23);
  auto roiOut = model->addOperand(&type21);
  auto classesOut = model->addOperand(&type6);
  auto batchSplitOut = model->addOperand(&type6);
  auto in = model->addOperand(&type18);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type1);
  auto param10 = model->addOperand(&type9);
  auto param11 = model->addOperand(&type9);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type1);
  auto layout = model->addOperand(&type10);
  auto featureMap = model->addOperand(&type17);
  auto op = model->addOperand(&type19);
  auto param14 = model->addOperand(&type1);
  auto out = model->addOperand(&type31);
  // Phase 2, operations
  static uint8_t scores_init[] = {137, 129};
  model->setOperandValue(scores, scores_init, sizeof(uint8_t) * 2);
  static uint16_t roi_init[] = {8, 8, 80, 80, 0, 0, 80, 80};
  model->setOperandValue(roi, roi_init, sizeof(uint16_t) * 8);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static float param2_init[] = {0.3f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t param3_init[] = {-1};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {0};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {0.4f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {1.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.3f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {2};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static float param10_init[] = {2.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {2.0f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t param12_init[] = {4};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {4};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static uint8_t op_init[] = {138, 148, 158, 168};
  model->setOperandValue(op, op_init, sizeof(uint8_t) * 4);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param1, param2, param3, param4, param5, param6, param7}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param8, param9, param10, param11, param12, param13, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_SUB, {featureMap, op, param14}, {out});
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
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::BOOL, {});
  OperandType type13(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type15(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 2, 2, 2});
  OperandType type25(Type::TENSOR_FLOAT16, {1, 1, 1, 2});
  OperandType type26(Type::FLOAT16, {});
  OperandType type27(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type29(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type32(Type::TENSOR_FLOAT16, {0});
  OperandType type6(Type::TENSOR_INT32, {0});
  OperandType type8(Type::TENSOR_INT32, {1});
  // Phase 1, operands
  auto scores = model->addOperand(&type29);
  auto roi = model->addOperand(&type27);
  auto param1 = model->addOperand(&type8);
  auto param2 = model->addOperand(&type26);
  auto param3 = model->addOperand(&type1);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type26);
  auto param6 = model->addOperand(&type26);
  auto param7 = model->addOperand(&type26);
  auto scoresOut = model->addOperand(&type32);
  auto roiOut = model->addOperand(&type28);
  auto classesOut = model->addOperand(&type6);
  auto batchSplitOut = model->addOperand(&type6);
  auto in = model->addOperand(&type25);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type1);
  auto param10 = model->addOperand(&type26);
  auto param11 = model->addOperand(&type26);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type1);
  auto layout = model->addOperand(&type10);
  auto featureMap = model->addOperand(&type24);
  auto op = model->addOperand(&type13);
  auto param14 = model->addOperand(&type1);
  auto out = model->addOperand(&type15);
  // Phase 2, operations
  static _Float16 scores_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores, scores_init, sizeof(_Float16) * 2);
  static _Float16 roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(_Float16) * 8);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static _Float16 param2_init[] = {0.30000001192092896f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t param3_init[] = {-1};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {0};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {0.4000000059604645f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {1.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.30000001192092896f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {2};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static _Float16 param10_init[] = {2.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {2.0f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t param12_init[] = {4};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {4};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static _Float16 op_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op, op_init, sizeof(_Float16) * 4);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param1, param2, param3, param4, param5, param6, param7}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param8, param9, param10, param11, param12, param13, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_SUB, {featureMap, op, param14}, {out});
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

