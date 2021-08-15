// clang-format off
// Generated file (from: sub_quantized_different_scales.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto input1 = model->addOperand(&type0);
  auto param = model->addOperand(&type1);
  auto output0 = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input0, input1, param}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {0}, 1.0f, 0);
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto input1 = model->addOperand(&type0);
  auto param = model->addOperand(&type1);
  auto output0 = model->addOperand(&type5);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input0, input1, param}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_2(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  // Phase 1, operands
  auto input01 = model->addOperand(&type0);
  auto input11 = model->addOperand(&type0);
  auto param1 = model->addOperand(&type1);
  auto output01 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input01, input11, param1}, {output01});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input01, input11},
    {output01});
  assert(model->isValid());
}

inline bool is_ignored_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_2(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {0}, 1.0f, 1);
  // Phase 1, operands
  auto input01 = model->addOperand(&type0);
  auto input11 = model->addOperand(&type0);
  auto param1 = model->addOperand(&type1);
  auto output01 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input01, input11, param1}, {output01});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input01, input11},
    {output01});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  // Phase 1, operands
  auto input02 = model->addOperand(&type0);
  auto input12 = model->addOperand(&type0);
  auto param2 = model->addOperand(&type1);
  auto output02 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input02, input12, param2}, {output02});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input02, input12},
    {output02});
  assert(model->isValid());
}

inline bool is_ignored_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_3(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {0}, 0.01f, 120);
  // Phase 1, operands
  auto input02 = model->addOperand(&type0);
  auto input12 = model->addOperand(&type0);
  auto param2 = model->addOperand(&type1);
  auto output02 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input02, input12, param2}, {output02});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input02, input12},
    {output02});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_4(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input03 = model->addOperand(&type0);
  auto input13 = model->addOperand(&type0);
  auto param3 = model->addOperand(&type1);
  auto output03 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input03, input13, param3}, {output03});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input03, input13},
    {output03});
  assert(model->isValid());
}

inline bool is_ignored_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_4(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {0}, 10.0f, 120);
  // Phase 1, operands
  auto input03 = model->addOperand(&type0);
  auto input13 = model->addOperand(&type0);
  auto param3 = model->addOperand(&type1);
  auto output03 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input03, input13, param3}, {output03});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input03, input13},
    {output03});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_5(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  // Phase 1, operands
  auto input04 = model->addOperand(&type0);
  auto input14 = model->addOperand(&type2);
  auto param4 = model->addOperand(&type1);
  auto output04 = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t param4_init[] = {0};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input04, input14, param4}, {output04});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input04, input14},
    {output04});
  assert(model->isValid());
}

inline bool is_ignored_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_5(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {0}, 1.0f, 0);
  // Phase 1, operands
  auto input04 = model->addOperand(&type0);
  auto input14 = model->addOperand(&type2);
  auto param4 = model->addOperand(&type1);
  auto output04 = model->addOperand(&type5);
  // Phase 2, operations
  static int32_t param4_init[] = {0};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input04, input14, param4}, {output04});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input04, input14},
    {output04});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_6(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  // Phase 1, operands
  auto input05 = model->addOperand(&type0);
  auto input15 = model->addOperand(&type2);
  auto param5 = model->addOperand(&type1);
  auto output05 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t param5_init[] = {0};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input05, input15, param5}, {output05});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input05, input15},
    {output05});
  assert(model->isValid());
}

inline bool is_ignored_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_6(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {0}, 1.0f, 1);
  // Phase 1, operands
  auto input05 = model->addOperand(&type0);
  auto input15 = model->addOperand(&type2);
  auto param5 = model->addOperand(&type1);
  auto output05 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param5_init[] = {0};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input05, input15, param5}, {output05});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input05, input15},
    {output05});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_7(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  // Phase 1, operands
  auto input06 = model->addOperand(&type0);
  auto input16 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type1);
  auto output06 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input06, input16, param6}, {output06});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input06, input16},
    {output06});
  assert(model->isValid());
}

inline bool is_ignored_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_7(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {0}, 0.01f, 120);
  // Phase 1, operands
  auto input06 = model->addOperand(&type0);
  auto input16 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type1);
  auto output06 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input06, input16, param6}, {output06});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input06, input16},
    {output06});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_8(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input07 = model->addOperand(&type0);
  auto input17 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type1);
  auto output07 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input07, input17, param7}, {output07});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input07, input17},
    {output07});
  assert(model->isValid());
}

inline bool is_ignored_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_8(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {0}, 10.0f, 120);
  // Phase 1, operands
  auto input07 = model->addOperand(&type0);
  auto input17 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type1);
  auto output07 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input07, input17, param7}, {output07});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input07, input17},
    {output07});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_9(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  // Phase 1, operands
  auto input08 = model->addOperand(&type0);
  auto input18 = model->addOperand(&type3);
  auto param8 = model->addOperand(&type1);
  auto output08 = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t param8_init[] = {0};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input08, input18, param8}, {output08});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input08, input18},
    {output08});
  assert(model->isValid());
}

inline bool is_ignored_9(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_9(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {0}, 1.0f, 0);
  // Phase 1, operands
  auto input08 = model->addOperand(&type0);
  auto input18 = model->addOperand(&type3);
  auto param8 = model->addOperand(&type1);
  auto output08 = model->addOperand(&type5);
  // Phase 2, operations
  static int32_t param8_init[] = {0};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input08, input18, param8}, {output08});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input08, input18},
    {output08});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_9(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_10(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  // Phase 1, operands
  auto input09 = model->addOperand(&type0);
  auto input19 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type1);
  auto output09 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input09, input19, param9}, {output09});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input09, input19},
    {output09});
  assert(model->isValid());
}

inline bool is_ignored_10(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_10(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {0}, 1.0f, 1);
  // Phase 1, operands
  auto input09 = model->addOperand(&type0);
  auto input19 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type1);
  auto output09 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input09, input19, param9}, {output09});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input09, input19},
    {output09});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_10(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_11(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  // Phase 1, operands
  auto input010 = model->addOperand(&type0);
  auto input110 = model->addOperand(&type3);
  auto param10 = model->addOperand(&type1);
  auto output010 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input010, input110, param10}, {output010});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input010, input110},
    {output010});
  assert(model->isValid());
}

inline bool is_ignored_11(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_11(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {0}, 0.01f, 120);
  // Phase 1, operands
  auto input010 = model->addOperand(&type0);
  auto input110 = model->addOperand(&type3);
  auto param10 = model->addOperand(&type1);
  auto output010 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input010, input110, param10}, {output010});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input010, input110},
    {output010});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_11(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_12(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input011 = model->addOperand(&type0);
  auto input111 = model->addOperand(&type3);
  auto param11 = model->addOperand(&type1);
  auto output011 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input011, input111, param11}, {output011});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input011, input111},
    {output011});
  assert(model->isValid());
}

inline bool is_ignored_12(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_12(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {0}, 10.0f, 120);
  // Phase 1, operands
  auto input011 = model->addOperand(&type0);
  auto input111 = model->addOperand(&type3);
  auto param11 = model->addOperand(&type1);
  auto output011 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input011, input111, param11}, {output011});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input011, input111},
    {output011});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_12(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_13(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input012 = model->addOperand(&type0);
  auto input112 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type1);
  auto output012 = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input012, input112, param12}, {output012});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input012, input112},
    {output012});
  assert(model->isValid());
}

inline bool is_ignored_13(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_13(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {0}, 1.0f, 0);
  // Phase 1, operands
  auto input012 = model->addOperand(&type0);
  auto input112 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type1);
  auto output012 = model->addOperand(&type5);
  // Phase 2, operations
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input012, input112, param12}, {output012});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input012, input112},
    {output012});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_13(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_14(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input013 = model->addOperand(&type0);
  auto input113 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type1);
  auto output013 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input013, input113, param13}, {output013});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input013, input113},
    {output013});
  assert(model->isValid());
}

inline bool is_ignored_14(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_14(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {0}, 1.0f, 1);
  // Phase 1, operands
  auto input013 = model->addOperand(&type0);
  auto input113 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type1);
  auto output013 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input013, input113, param13}, {output013});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input013, input113},
    {output013});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_14(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_15(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input014 = model->addOperand(&type0);
  auto input114 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type1);
  auto output014 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input014, input114, param14}, {output014});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input014, input114},
    {output014});
  assert(model->isValid());
}

inline bool is_ignored_15(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_15(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {0}, 0.01f, 120);
  // Phase 1, operands
  auto input014 = model->addOperand(&type0);
  auto input114 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type1);
  auto output014 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input014, input114, param14}, {output014});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input014, input114},
    {output014});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_15(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_16(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input015 = model->addOperand(&type0);
  auto input115 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type1);
  auto output015 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input015, input115, param15}, {output015});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input015, input115},
    {output015});
  assert(model->isValid());
}

inline bool is_ignored_16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_16(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {0}, 10.0f, 120);
  // Phase 1, operands
  auto input015 = model->addOperand(&type0);
  auto input115 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type1);
  auto output015 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input015, input115, param15}, {output015});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input015, input115},
    {output015});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_17(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  // Phase 1, operands
  auto input016 = model->addOperand(&type2);
  auto input116 = model->addOperand(&type0);
  auto param16 = model->addOperand(&type1);
  auto output016 = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input016, input116, param16}, {output016});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input016, input116},
    {output016});
  assert(model->isValid());
}

inline bool is_ignored_17(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_17(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {0}, 1.0f, 0);
  // Phase 1, operands
  auto input016 = model->addOperand(&type2);
  auto input116 = model->addOperand(&type0);
  auto param16 = model->addOperand(&type1);
  auto output016 = model->addOperand(&type5);
  // Phase 2, operations
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input016, input116, param16}, {output016});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input016, input116},
    {output016});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_17(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_18(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  // Phase 1, operands
  auto input017 = model->addOperand(&type2);
  auto input117 = model->addOperand(&type0);
  auto param17 = model->addOperand(&type1);
  auto output017 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input017, input117, param17}, {output017});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input017, input117},
    {output017});
  assert(model->isValid());
}

inline bool is_ignored_18(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_18(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {0}, 1.0f, 1);
  // Phase 1, operands
  auto input017 = model->addOperand(&type2);
  auto input117 = model->addOperand(&type0);
  auto param17 = model->addOperand(&type1);
  auto output017 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input017, input117, param17}, {output017});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input017, input117},
    {output017});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_18(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_19(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  // Phase 1, operands
  auto input018 = model->addOperand(&type2);
  auto input118 = model->addOperand(&type0);
  auto param18 = model->addOperand(&type1);
  auto output018 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input018, input118, param18}, {output018});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input018, input118},
    {output018});
  assert(model->isValid());
}

inline bool is_ignored_19(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_19(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {0}, 0.01f, 120);
  // Phase 1, operands
  auto input018 = model->addOperand(&type2);
  auto input118 = model->addOperand(&type0);
  auto param18 = model->addOperand(&type1);
  auto output018 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input018, input118, param18}, {output018});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input018, input118},
    {output018});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_19(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_20(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input019 = model->addOperand(&type2);
  auto input119 = model->addOperand(&type0);
  auto param19 = model->addOperand(&type1);
  auto output019 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input019, input119, param19}, {output019});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input019, input119},
    {output019});
  assert(model->isValid());
}

inline bool is_ignored_20(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_20(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {0}, 10.0f, 120);
  // Phase 1, operands
  auto input019 = model->addOperand(&type2);
  auto input119 = model->addOperand(&type0);
  auto param19 = model->addOperand(&type1);
  auto output019 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input019, input119, param19}, {output019});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input019, input119},
    {output019});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_20(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_21(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  // Phase 1, operands
  auto input020 = model->addOperand(&type2);
  auto input120 = model->addOperand(&type2);
  auto param20 = model->addOperand(&type1);
  auto output020 = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input020, input120, param20}, {output020});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input020, input120},
    {output020});
  assert(model->isValid());
}

inline bool is_ignored_21(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_21(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {0}, 1.0f, 0);
  // Phase 1, operands
  auto input020 = model->addOperand(&type2);
  auto input120 = model->addOperand(&type2);
  auto param20 = model->addOperand(&type1);
  auto output020 = model->addOperand(&type5);
  // Phase 2, operations
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input020, input120, param20}, {output020});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input020, input120},
    {output020});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_21(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_22(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  // Phase 1, operands
  auto input021 = model->addOperand(&type2);
  auto input121 = model->addOperand(&type2);
  auto param21 = model->addOperand(&type1);
  auto output021 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input021, input121, param21}, {output021});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input021, input121},
    {output021});
  assert(model->isValid());
}

inline bool is_ignored_22(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_22(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {0}, 1.0f, 1);
  // Phase 1, operands
  auto input021 = model->addOperand(&type2);
  auto input121 = model->addOperand(&type2);
  auto param21 = model->addOperand(&type1);
  auto output021 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input021, input121, param21}, {output021});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input021, input121},
    {output021});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_22(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_23(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  // Phase 1, operands
  auto input022 = model->addOperand(&type2);
  auto input122 = model->addOperand(&type2);
  auto param22 = model->addOperand(&type1);
  auto output022 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input022, input122, param22}, {output022});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input022, input122},
    {output022});
  assert(model->isValid());
}

inline bool is_ignored_23(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_23(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {0}, 0.01f, 120);
  // Phase 1, operands
  auto input022 = model->addOperand(&type2);
  auto input122 = model->addOperand(&type2);
  auto param22 = model->addOperand(&type1);
  auto output022 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input022, input122, param22}, {output022});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input022, input122},
    {output022});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_23(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_24(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input023 = model->addOperand(&type2);
  auto input123 = model->addOperand(&type2);
  auto param23 = model->addOperand(&type1);
  auto output023 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input023, input123, param23}, {output023});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input023, input123},
    {output023});
  assert(model->isValid());
}

inline bool is_ignored_24(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_24(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {0}, 10.0f, 120);
  // Phase 1, operands
  auto input023 = model->addOperand(&type2);
  auto input123 = model->addOperand(&type2);
  auto param23 = model->addOperand(&type1);
  auto output023 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input023, input123, param23}, {output023});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input023, input123},
    {output023});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_24(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_25(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  // Phase 1, operands
  auto input024 = model->addOperand(&type2);
  auto input124 = model->addOperand(&type3);
  auto param24 = model->addOperand(&type1);
  auto output024 = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input024, input124, param24}, {output024});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input024, input124},
    {output024});
  assert(model->isValid());
}

inline bool is_ignored_25(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_25(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {0}, 1.0f, 0);
  // Phase 1, operands
  auto input024 = model->addOperand(&type2);
  auto input124 = model->addOperand(&type3);
  auto param24 = model->addOperand(&type1);
  auto output024 = model->addOperand(&type5);
  // Phase 2, operations
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input024, input124, param24}, {output024});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input024, input124},
    {output024});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_25(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_26(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  // Phase 1, operands
  auto input025 = model->addOperand(&type2);
  auto input125 = model->addOperand(&type3);
  auto param25 = model->addOperand(&type1);
  auto output025 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t param25_init[] = {0};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input025, input125, param25}, {output025});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input025, input125},
    {output025});
  assert(model->isValid());
}

inline bool is_ignored_26(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_26(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {0}, 1.0f, 1);
  // Phase 1, operands
  auto input025 = model->addOperand(&type2);
  auto input125 = model->addOperand(&type3);
  auto param25 = model->addOperand(&type1);
  auto output025 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param25_init[] = {0};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input025, input125, param25}, {output025});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input025, input125},
    {output025});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_26(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_27(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  // Phase 1, operands
  auto input026 = model->addOperand(&type2);
  auto input126 = model->addOperand(&type3);
  auto param26 = model->addOperand(&type1);
  auto output026 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param26_init[] = {0};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input026, input126, param26}, {output026});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input026, input126},
    {output026});
  assert(model->isValid());
}

inline bool is_ignored_27(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_27(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {0}, 0.01f, 120);
  // Phase 1, operands
  auto input026 = model->addOperand(&type2);
  auto input126 = model->addOperand(&type3);
  auto param26 = model->addOperand(&type1);
  auto output026 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t param26_init[] = {0};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input026, input126, param26}, {output026});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input026, input126},
    {output026});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_27(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_28(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input027 = model->addOperand(&type2);
  auto input127 = model->addOperand(&type3);
  auto param27 = model->addOperand(&type1);
  auto output027 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input027, input127, param27}, {output027});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input027, input127},
    {output027});
  assert(model->isValid());
}

inline bool is_ignored_28(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_28(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {0}, 10.0f, 120);
  // Phase 1, operands
  auto input027 = model->addOperand(&type2);
  auto input127 = model->addOperand(&type3);
  auto param27 = model->addOperand(&type1);
  auto output027 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input027, input127, param27}, {output027});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input027, input127},
    {output027});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_28(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_29(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input028 = model->addOperand(&type2);
  auto input128 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type1);
  auto output028 = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input028, input128, param28}, {output028});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input028, input128},
    {output028});
  assert(model->isValid());
}

inline bool is_ignored_29(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_29(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {0}, 1.0f, 0);
  // Phase 1, operands
  auto input028 = model->addOperand(&type2);
  auto input128 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type1);
  auto output028 = model->addOperand(&type5);
  // Phase 2, operations
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input028, input128, param28}, {output028});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input028, input128},
    {output028});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_29(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_30(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input029 = model->addOperand(&type2);
  auto input129 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type1);
  auto output029 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t param29_init[] = {0};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input029, input129, param29}, {output029});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input029, input129},
    {output029});
  assert(model->isValid());
}

inline bool is_ignored_30(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_30(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {0}, 1.0f, 1);
  // Phase 1, operands
  auto input029 = model->addOperand(&type2);
  auto input129 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type1);
  auto output029 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param29_init[] = {0};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input029, input129, param29}, {output029});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input029, input129},
    {output029});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_30(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_31(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input030 = model->addOperand(&type2);
  auto input130 = model->addOperand(&type4);
  auto param30 = model->addOperand(&type1);
  auto output030 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param30_init[] = {0};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input030, input130, param30}, {output030});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input030, input130},
    {output030});
  assert(model->isValid());
}

inline bool is_ignored_31(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_31(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {0}, 0.01f, 120);
  // Phase 1, operands
  auto input030 = model->addOperand(&type2);
  auto input130 = model->addOperand(&type4);
  auto param30 = model->addOperand(&type1);
  auto output030 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t param30_init[] = {0};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input030, input130, param30}, {output030});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input030, input130},
    {output030});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_31(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_32(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input031 = model->addOperand(&type2);
  auto input131 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type1);
  auto output031 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t param31_init[] = {0};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input031, input131, param31}, {output031});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input031, input131},
    {output031});
  assert(model->isValid());
}

inline bool is_ignored_32(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_32(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {0}, 10.0f, 120);
  // Phase 1, operands
  auto input031 = model->addOperand(&type2);
  auto input131 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type1);
  auto output031 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param31_init[] = {0};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input031, input131, param31}, {output031});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input031, input131},
    {output031});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_32(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_33(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  // Phase 1, operands
  auto input032 = model->addOperand(&type3);
  auto input132 = model->addOperand(&type0);
  auto param32 = model->addOperand(&type1);
  auto output032 = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t param32_init[] = {0};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input032, input132, param32}, {output032});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input032, input132},
    {output032});
  assert(model->isValid());
}

inline bool is_ignored_33(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_33(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {0}, 1.0f, 0);
  // Phase 1, operands
  auto input032 = model->addOperand(&type3);
  auto input132 = model->addOperand(&type0);
  auto param32 = model->addOperand(&type1);
  auto output032 = model->addOperand(&type5);
  // Phase 2, operations
  static int32_t param32_init[] = {0};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input032, input132, param32}, {output032});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input032, input132},
    {output032});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_33(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_34(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  // Phase 1, operands
  auto input033 = model->addOperand(&type3);
  auto input133 = model->addOperand(&type0);
  auto param33 = model->addOperand(&type1);
  auto output033 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input033, input133, param33}, {output033});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input033, input133},
    {output033});
  assert(model->isValid());
}

inline bool is_ignored_34(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_34(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {0}, 1.0f, 1);
  // Phase 1, operands
  auto input033 = model->addOperand(&type3);
  auto input133 = model->addOperand(&type0);
  auto param33 = model->addOperand(&type1);
  auto output033 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input033, input133, param33}, {output033});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input033, input133},
    {output033});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_34(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_35(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  // Phase 1, operands
  auto input034 = model->addOperand(&type3);
  auto input134 = model->addOperand(&type0);
  auto param34 = model->addOperand(&type1);
  auto output034 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param34_init[] = {0};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input034, input134, param34}, {output034});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input034, input134},
    {output034});
  assert(model->isValid());
}

inline bool is_ignored_35(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_35(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {0}, 0.01f, 120);
  // Phase 1, operands
  auto input034 = model->addOperand(&type3);
  auto input134 = model->addOperand(&type0);
  auto param34 = model->addOperand(&type1);
  auto output034 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t param34_init[] = {0};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input034, input134, param34}, {output034});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input034, input134},
    {output034});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_35(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_36(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input035 = model->addOperand(&type3);
  auto input135 = model->addOperand(&type0);
  auto param35 = model->addOperand(&type1);
  auto output035 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t param35_init[] = {0};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input035, input135, param35}, {output035});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input035, input135},
    {output035});
  assert(model->isValid());
}

inline bool is_ignored_36(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_36(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {0}, 10.0f, 120);
  // Phase 1, operands
  auto input035 = model->addOperand(&type3);
  auto input135 = model->addOperand(&type0);
  auto param35 = model->addOperand(&type1);
  auto output035 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param35_init[] = {0};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input035, input135, param35}, {output035});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input035, input135},
    {output035});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_36(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_37(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  // Phase 1, operands
  auto input036 = model->addOperand(&type3);
  auto input136 = model->addOperand(&type2);
  auto param36 = model->addOperand(&type1);
  auto output036 = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t param36_init[] = {0};
  model->setOperandValue(param36, param36_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input036, input136, param36}, {output036});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input036, input136},
    {output036});
  assert(model->isValid());
}

inline bool is_ignored_37(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_37(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {0}, 1.0f, 0);
  // Phase 1, operands
  auto input036 = model->addOperand(&type3);
  auto input136 = model->addOperand(&type2);
  auto param36 = model->addOperand(&type1);
  auto output036 = model->addOperand(&type5);
  // Phase 2, operations
  static int32_t param36_init[] = {0};
  model->setOperandValue(param36, param36_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input036, input136, param36}, {output036});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input036, input136},
    {output036});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_37(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_38(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  // Phase 1, operands
  auto input037 = model->addOperand(&type3);
  auto input137 = model->addOperand(&type2);
  auto param37 = model->addOperand(&type1);
  auto output037 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t param37_init[] = {0};
  model->setOperandValue(param37, param37_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input037, input137, param37}, {output037});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input037, input137},
    {output037});
  assert(model->isValid());
}

inline bool is_ignored_38(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_38(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {0}, 1.0f, 1);
  // Phase 1, operands
  auto input037 = model->addOperand(&type3);
  auto input137 = model->addOperand(&type2);
  auto param37 = model->addOperand(&type1);
  auto output037 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param37_init[] = {0};
  model->setOperandValue(param37, param37_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input037, input137, param37}, {output037});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input037, input137},
    {output037});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_38(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_39(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  // Phase 1, operands
  auto input038 = model->addOperand(&type3);
  auto input138 = model->addOperand(&type2);
  auto param38 = model->addOperand(&type1);
  auto output038 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param38_init[] = {0};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input038, input138, param38}, {output038});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input038, input138},
    {output038});
  assert(model->isValid());
}

inline bool is_ignored_39(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_39(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {0}, 0.01f, 120);
  // Phase 1, operands
  auto input038 = model->addOperand(&type3);
  auto input138 = model->addOperand(&type2);
  auto param38 = model->addOperand(&type1);
  auto output038 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t param38_init[] = {0};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input038, input138, param38}, {output038});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input038, input138},
    {output038});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_39(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_40(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input039 = model->addOperand(&type3);
  auto input139 = model->addOperand(&type2);
  auto param39 = model->addOperand(&type1);
  auto output039 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t param39_init[] = {0};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input039, input139, param39}, {output039});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input039, input139},
    {output039});
  assert(model->isValid());
}

inline bool is_ignored_40(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_40(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {0}, 10.0f, 120);
  // Phase 1, operands
  auto input039 = model->addOperand(&type3);
  auto input139 = model->addOperand(&type2);
  auto param39 = model->addOperand(&type1);
  auto output039 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param39_init[] = {0};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input039, input139, param39}, {output039});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input039, input139},
    {output039});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_40(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_41(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  // Phase 1, operands
  auto input040 = model->addOperand(&type3);
  auto input140 = model->addOperand(&type3);
  auto param40 = model->addOperand(&type1);
  auto output040 = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t param40_init[] = {0};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input040, input140, param40}, {output040});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input040, input140},
    {output040});
  assert(model->isValid());
}

inline bool is_ignored_41(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_41(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {0}, 1.0f, 0);
  // Phase 1, operands
  auto input040 = model->addOperand(&type3);
  auto input140 = model->addOperand(&type3);
  auto param40 = model->addOperand(&type1);
  auto output040 = model->addOperand(&type5);
  // Phase 2, operations
  static int32_t param40_init[] = {0};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input040, input140, param40}, {output040});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input040, input140},
    {output040});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_41(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_42(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  // Phase 1, operands
  auto input041 = model->addOperand(&type3);
  auto input141 = model->addOperand(&type3);
  auto param41 = model->addOperand(&type1);
  auto output041 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t param41_init[] = {0};
  model->setOperandValue(param41, param41_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input041, input141, param41}, {output041});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input041, input141},
    {output041});
  assert(model->isValid());
}

inline bool is_ignored_42(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_42(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {0}, 1.0f, 1);
  // Phase 1, operands
  auto input041 = model->addOperand(&type3);
  auto input141 = model->addOperand(&type3);
  auto param41 = model->addOperand(&type1);
  auto output041 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param41_init[] = {0};
  model->setOperandValue(param41, param41_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input041, input141, param41}, {output041});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input041, input141},
    {output041});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_42(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_43(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  // Phase 1, operands
  auto input042 = model->addOperand(&type3);
  auto input142 = model->addOperand(&type3);
  auto param42 = model->addOperand(&type1);
  auto output042 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param42_init[] = {0};
  model->setOperandValue(param42, param42_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input042, input142, param42}, {output042});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input042, input142},
    {output042});
  assert(model->isValid());
}

inline bool is_ignored_43(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_43(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {0}, 0.01f, 120);
  // Phase 1, operands
  auto input042 = model->addOperand(&type3);
  auto input142 = model->addOperand(&type3);
  auto param42 = model->addOperand(&type1);
  auto output042 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t param42_init[] = {0};
  model->setOperandValue(param42, param42_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input042, input142, param42}, {output042});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input042, input142},
    {output042});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_43(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_44(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input043 = model->addOperand(&type3);
  auto input143 = model->addOperand(&type3);
  auto param43 = model->addOperand(&type1);
  auto output043 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t param43_init[] = {0};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input043, input143, param43}, {output043});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input043, input143},
    {output043});
  assert(model->isValid());
}

inline bool is_ignored_44(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_44(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {0}, 10.0f, 120);
  // Phase 1, operands
  auto input043 = model->addOperand(&type3);
  auto input143 = model->addOperand(&type3);
  auto param43 = model->addOperand(&type1);
  auto output043 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param43_init[] = {0};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input043, input143, param43}, {output043});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input043, input143},
    {output043});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_44(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_45(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input044 = model->addOperand(&type3);
  auto input144 = model->addOperand(&type4);
  auto param44 = model->addOperand(&type1);
  auto output044 = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t param44_init[] = {0};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input044, input144, param44}, {output044});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input044, input144},
    {output044});
  assert(model->isValid());
}

inline bool is_ignored_45(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_45(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {0}, 1.0f, 0);
  // Phase 1, operands
  auto input044 = model->addOperand(&type3);
  auto input144 = model->addOperand(&type4);
  auto param44 = model->addOperand(&type1);
  auto output044 = model->addOperand(&type5);
  // Phase 2, operations
  static int32_t param44_init[] = {0};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input044, input144, param44}, {output044});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input044, input144},
    {output044});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_45(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_46(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input045 = model->addOperand(&type3);
  auto input145 = model->addOperand(&type4);
  auto param45 = model->addOperand(&type1);
  auto output045 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t param45_init[] = {0};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input045, input145, param45}, {output045});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input045, input145},
    {output045});
  assert(model->isValid());
}

inline bool is_ignored_46(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_46(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {0}, 1.0f, 1);
  // Phase 1, operands
  auto input045 = model->addOperand(&type3);
  auto input145 = model->addOperand(&type4);
  auto param45 = model->addOperand(&type1);
  auto output045 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param45_init[] = {0};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input045, input145, param45}, {output045});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input045, input145},
    {output045});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_46(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_47(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input046 = model->addOperand(&type3);
  auto input146 = model->addOperand(&type4);
  auto param46 = model->addOperand(&type1);
  auto output046 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param46_init[] = {0};
  model->setOperandValue(param46, param46_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input046, input146, param46}, {output046});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input046, input146},
    {output046});
  assert(model->isValid());
}

inline bool is_ignored_47(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_47(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {0}, 0.01f, 120);
  // Phase 1, operands
  auto input046 = model->addOperand(&type3);
  auto input146 = model->addOperand(&type4);
  auto param46 = model->addOperand(&type1);
  auto output046 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t param46_init[] = {0};
  model->setOperandValue(param46, param46_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input046, input146, param46}, {output046});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input046, input146},
    {output046});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_47(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_48(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input047 = model->addOperand(&type3);
  auto input147 = model->addOperand(&type4);
  auto param47 = model->addOperand(&type1);
  auto output047 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t param47_init[] = {0};
  model->setOperandValue(param47, param47_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input047, input147, param47}, {output047});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input047, input147},
    {output047});
  assert(model->isValid());
}

inline bool is_ignored_48(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_48(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {0}, 10.0f, 120);
  // Phase 1, operands
  auto input047 = model->addOperand(&type3);
  auto input147 = model->addOperand(&type4);
  auto param47 = model->addOperand(&type1);
  auto output047 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param47_init[] = {0};
  model->setOperandValue(param47, param47_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input047, input147, param47}, {output047});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input047, input147},
    {output047});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_48(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_49(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input048 = model->addOperand(&type4);
  auto input148 = model->addOperand(&type0);
  auto param48 = model->addOperand(&type1);
  auto output048 = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t param48_init[] = {0};
  model->setOperandValue(param48, param48_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input048, input148, param48}, {output048});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input048, input148},
    {output048});
  assert(model->isValid());
}

inline bool is_ignored_49(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_49(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {0}, 1.0f, 0);
  // Phase 1, operands
  auto input048 = model->addOperand(&type4);
  auto input148 = model->addOperand(&type0);
  auto param48 = model->addOperand(&type1);
  auto output048 = model->addOperand(&type5);
  // Phase 2, operations
  static int32_t param48_init[] = {0};
  model->setOperandValue(param48, param48_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input048, input148, param48}, {output048});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input048, input148},
    {output048});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_49(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_50(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input049 = model->addOperand(&type4);
  auto input149 = model->addOperand(&type0);
  auto param49 = model->addOperand(&type1);
  auto output049 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t param49_init[] = {0};
  model->setOperandValue(param49, param49_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input049, input149, param49}, {output049});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input049, input149},
    {output049});
  assert(model->isValid());
}

inline bool is_ignored_50(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_50(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {0}, 1.0f, 1);
  // Phase 1, operands
  auto input049 = model->addOperand(&type4);
  auto input149 = model->addOperand(&type0);
  auto param49 = model->addOperand(&type1);
  auto output049 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param49_init[] = {0};
  model->setOperandValue(param49, param49_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input049, input149, param49}, {output049});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input049, input149},
    {output049});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_50(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_51(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input050 = model->addOperand(&type4);
  auto input150 = model->addOperand(&type0);
  auto param50 = model->addOperand(&type1);
  auto output050 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param50_init[] = {0};
  model->setOperandValue(param50, param50_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input050, input150, param50}, {output050});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input050, input150},
    {output050});
  assert(model->isValid());
}

inline bool is_ignored_51(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_51(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {0}, 0.01f, 120);
  // Phase 1, operands
  auto input050 = model->addOperand(&type4);
  auto input150 = model->addOperand(&type0);
  auto param50 = model->addOperand(&type1);
  auto output050 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t param50_init[] = {0};
  model->setOperandValue(param50, param50_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input050, input150, param50}, {output050});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input050, input150},
    {output050});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_51(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_52(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input051 = model->addOperand(&type4);
  auto input151 = model->addOperand(&type0);
  auto param51 = model->addOperand(&type1);
  auto output051 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t param51_init[] = {0};
  model->setOperandValue(param51, param51_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input051, input151, param51}, {output051});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input051, input151},
    {output051});
  assert(model->isValid());
}

inline bool is_ignored_52(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_52(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {0}, 10.0f, 120);
  // Phase 1, operands
  auto input051 = model->addOperand(&type4);
  auto input151 = model->addOperand(&type0);
  auto param51 = model->addOperand(&type1);
  auto output051 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param51_init[] = {0};
  model->setOperandValue(param51, param51_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input051, input151, param51}, {output051});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input051, input151},
    {output051});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_52(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_53(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input052 = model->addOperand(&type4);
  auto input152 = model->addOperand(&type2);
  auto param52 = model->addOperand(&type1);
  auto output052 = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t param52_init[] = {0};
  model->setOperandValue(param52, param52_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input052, input152, param52}, {output052});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input052, input152},
    {output052});
  assert(model->isValid());
}

inline bool is_ignored_53(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_53(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {0}, 1.0f, 0);
  // Phase 1, operands
  auto input052 = model->addOperand(&type4);
  auto input152 = model->addOperand(&type2);
  auto param52 = model->addOperand(&type1);
  auto output052 = model->addOperand(&type5);
  // Phase 2, operations
  static int32_t param52_init[] = {0};
  model->setOperandValue(param52, param52_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input052, input152, param52}, {output052});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input052, input152},
    {output052});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_53(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_54(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input053 = model->addOperand(&type4);
  auto input153 = model->addOperand(&type2);
  auto param53 = model->addOperand(&type1);
  auto output053 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t param53_init[] = {0};
  model->setOperandValue(param53, param53_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input053, input153, param53}, {output053});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input053, input153},
    {output053});
  assert(model->isValid());
}

inline bool is_ignored_54(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_54(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {0}, 1.0f, 1);
  // Phase 1, operands
  auto input053 = model->addOperand(&type4);
  auto input153 = model->addOperand(&type2);
  auto param53 = model->addOperand(&type1);
  auto output053 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param53_init[] = {0};
  model->setOperandValue(param53, param53_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input053, input153, param53}, {output053});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input053, input153},
    {output053});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_54(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_55(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input054 = model->addOperand(&type4);
  auto input154 = model->addOperand(&type2);
  auto param54 = model->addOperand(&type1);
  auto output054 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param54_init[] = {0};
  model->setOperandValue(param54, param54_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input054, input154, param54}, {output054});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input054, input154},
    {output054});
  assert(model->isValid());
}

inline bool is_ignored_55(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_55(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {0}, 0.01f, 120);
  // Phase 1, operands
  auto input054 = model->addOperand(&type4);
  auto input154 = model->addOperand(&type2);
  auto param54 = model->addOperand(&type1);
  auto output054 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t param54_init[] = {0};
  model->setOperandValue(param54, param54_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input054, input154, param54}, {output054});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input054, input154},
    {output054});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_55(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_56(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input055 = model->addOperand(&type4);
  auto input155 = model->addOperand(&type2);
  auto param55 = model->addOperand(&type1);
  auto output055 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t param55_init[] = {0};
  model->setOperandValue(param55, param55_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input055, input155, param55}, {output055});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input055, input155},
    {output055});
  assert(model->isValid());
}

inline bool is_ignored_56(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_56(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {0}, 10.0f, 120);
  // Phase 1, operands
  auto input055 = model->addOperand(&type4);
  auto input155 = model->addOperand(&type2);
  auto param55 = model->addOperand(&type1);
  auto output055 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param55_init[] = {0};
  model->setOperandValue(param55, param55_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input055, input155, param55}, {output055});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input055, input155},
    {output055});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_56(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_57(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input056 = model->addOperand(&type4);
  auto input156 = model->addOperand(&type3);
  auto param56 = model->addOperand(&type1);
  auto output056 = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t param56_init[] = {0};
  model->setOperandValue(param56, param56_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input056, input156, param56}, {output056});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input056, input156},
    {output056});
  assert(model->isValid());
}

inline bool is_ignored_57(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_57(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {0}, 1.0f, 0);
  // Phase 1, operands
  auto input056 = model->addOperand(&type4);
  auto input156 = model->addOperand(&type3);
  auto param56 = model->addOperand(&type1);
  auto output056 = model->addOperand(&type5);
  // Phase 2, operations
  static int32_t param56_init[] = {0};
  model->setOperandValue(param56, param56_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input056, input156, param56}, {output056});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input056, input156},
    {output056});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_57(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_58(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input057 = model->addOperand(&type4);
  auto input157 = model->addOperand(&type3);
  auto param57 = model->addOperand(&type1);
  auto output057 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t param57_init[] = {0};
  model->setOperandValue(param57, param57_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input057, input157, param57}, {output057});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input057, input157},
    {output057});
  assert(model->isValid());
}

inline bool is_ignored_58(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_58(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {0}, 1.0f, 1);
  // Phase 1, operands
  auto input057 = model->addOperand(&type4);
  auto input157 = model->addOperand(&type3);
  auto param57 = model->addOperand(&type1);
  auto output057 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param57_init[] = {0};
  model->setOperandValue(param57, param57_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input057, input157, param57}, {output057});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input057, input157},
    {output057});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_58(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_59(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input058 = model->addOperand(&type4);
  auto input158 = model->addOperand(&type3);
  auto param58 = model->addOperand(&type1);
  auto output058 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param58_init[] = {0};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input058, input158, param58}, {output058});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input058, input158},
    {output058});
  assert(model->isValid());
}

inline bool is_ignored_59(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_59(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {0}, 0.01f, 120);
  // Phase 1, operands
  auto input058 = model->addOperand(&type4);
  auto input158 = model->addOperand(&type3);
  auto param58 = model->addOperand(&type1);
  auto output058 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t param58_init[] = {0};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input058, input158, param58}, {output058});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input058, input158},
    {output058});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_59(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_60(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input059 = model->addOperand(&type4);
  auto input159 = model->addOperand(&type3);
  auto param59 = model->addOperand(&type1);
  auto output059 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t param59_init[] = {0};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input059, input159, param59}, {output059});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input059, input159},
    {output059});
  assert(model->isValid());
}

inline bool is_ignored_60(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_60(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {0}, 10.0f, 120);
  // Phase 1, operands
  auto input059 = model->addOperand(&type4);
  auto input159 = model->addOperand(&type3);
  auto param59 = model->addOperand(&type1);
  auto output059 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param59_init[] = {0};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input059, input159, param59}, {output059});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input059, input159},
    {output059});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_60(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_61(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input060 = model->addOperand(&type4);
  auto input160 = model->addOperand(&type4);
  auto param60 = model->addOperand(&type1);
  auto output060 = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t param60_init[] = {0};
  model->setOperandValue(param60, param60_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input060, input160, param60}, {output060});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input060, input160},
    {output060});
  assert(model->isValid());
}

inline bool is_ignored_61(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_61(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {0}, 1.0f, 0);
  // Phase 1, operands
  auto input060 = model->addOperand(&type4);
  auto input160 = model->addOperand(&type4);
  auto param60 = model->addOperand(&type1);
  auto output060 = model->addOperand(&type5);
  // Phase 2, operations
  static int32_t param60_init[] = {0};
  model->setOperandValue(param60, param60_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input060, input160, param60}, {output060});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input060, input160},
    {output060});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_61(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_62(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {144}, 1.0f, 1);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input061 = model->addOperand(&type4);
  auto input161 = model->addOperand(&type4);
  auto param61 = model->addOperand(&type1);
  auto output061 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t param61_init[] = {0};
  model->setOperandValue(param61, param61_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input061, input161, param61}, {output061});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input061, input161},
    {output061});
  assert(model->isValid());
}

inline bool is_ignored_62(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_62(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {0}, 1.0f, 1);
  // Phase 1, operands
  auto input061 = model->addOperand(&type4);
  auto input161 = model->addOperand(&type4);
  auto param61 = model->addOperand(&type1);
  auto output061 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param61_init[] = {0};
  model->setOperandValue(param61, param61_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input061, input161, param61}, {output061});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input061, input161},
    {output061});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_62(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_63(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {144}, 0.01f, 120);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input062 = model->addOperand(&type4);
  auto input162 = model->addOperand(&type4);
  auto param62 = model->addOperand(&type1);
  auto output062 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param62_init[] = {0};
  model->setOperandValue(param62, param62_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input062, input162, param62}, {output062});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input062, input162},
    {output062});
  assert(model->isValid());
}

inline bool is_ignored_63(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_63(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {0}, 0.01f, 120);
  // Phase 1, operands
  auto input062 = model->addOperand(&type4);
  auto input162 = model->addOperand(&type4);
  auto param62 = model->addOperand(&type1);
  auto output062 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t param62_init[] = {0};
  model->setOperandValue(param62, param62_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input062, input162, param62}, {output062});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input062, input162},
    {output062});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_63(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_64(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  // Phase 1, operands
  auto input063 = model->addOperand(&type4);
  auto input163 = model->addOperand(&type4);
  auto param63 = model->addOperand(&type1);
  auto output063 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t param63_init[] = {0};
  model->setOperandValue(param63, param63_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input063, input163, param63}, {output063});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input063, input163},
    {output063});
  assert(model->isValid());
}

inline bool is_ignored_64(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_64(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {144}, 10.0f, 120);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {0}, 10.0f, 120);
  // Phase 1, operands
  auto input063 = model->addOperand(&type4);
  auto input163 = model->addOperand(&type4);
  auto param63 = model->addOperand(&type1);
  auto output063 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param63_init[] = {0};
  model->setOperandValue(param63, param63_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SUB, {input063, input163, param63}, {output063});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input063, input163},
    {output063});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_64(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

