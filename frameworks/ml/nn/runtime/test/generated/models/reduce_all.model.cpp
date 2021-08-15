// clang-format off
// Generated file (from: reduce_all.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_BOOL8, {1});
  OperandType type1(Type::TENSOR_INT32, {1});
  OperandType type2(Type::BOOL, {});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto output0 = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static bool8 param1_init[] = {true};
  model->setOperandValue(param1, param1_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_REDUCE_ALL, {input0, param, param1}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_BOOL8, {1});
  OperandType type1(Type::TENSOR_INT32, {1});
  OperandType type2(Type::BOOL, {});
  OperandType type8(Type::TENSOR_BOOL8, {0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto output0 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static bool8 param1_init[] = {true};
  model->setOperandValue(param1, param1_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_REDUCE_ALL, {input0, param, param1}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_2(Model *model) {
  OperandType type2(Type::BOOL, {});
  OperandType type3(Type::TENSOR_BOOL8, {2, 3, 2});
  OperandType type4(Type::TENSOR_BOOL8, {2});
  OperandType type5(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input01 = model->addOperand(&type3);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type2);
  auto output01 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t param2_init[] = {1, 0, -3, -3};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 4);
  static bool8 param3_init[] = {false};
  model->setOperandValue(param3, param3_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_REDUCE_ALL, {input01, param2, param3}, {output01});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input01},
    {output01});
  assert(model->isValid());
}

inline bool is_ignored_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_2(Model *model) {
  OperandType type2(Type::BOOL, {});
  OperandType type3(Type::TENSOR_BOOL8, {2, 3, 2});
  OperandType type5(Type::TENSOR_INT32, {4});
  OperandType type8(Type::TENSOR_BOOL8, {0});
  // Phase 1, operands
  auto input01 = model->addOperand(&type3);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type2);
  auto output01 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param2_init[] = {1, 0, -3, -3};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 4);
  static bool8 param3_init[] = {false};
  model->setOperandValue(param3, param3_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_REDUCE_ALL, {input01, param2, param3}, {output01});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input01},
    {output01});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3(Model *model) {
  OperandType type2(Type::BOOL, {});
  OperandType type3(Type::TENSOR_BOOL8, {2, 3, 2});
  OperandType type6(Type::TENSOR_BOOL8, {1, 3, 1});
  OperandType type7(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto input02 = model->addOperand(&type3);
  auto param4 = model->addOperand(&type7);
  auto param5 = model->addOperand(&type2);
  auto output02 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param4_init[] = {0, 2};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 2);
  static bool8 param5_init[] = {true};
  model->setOperandValue(param5, param5_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_REDUCE_ALL, {input02, param4, param5}, {output02});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input02},
    {output02});
  assert(model->isValid());
}

inline bool is_ignored_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_3(Model *model) {
  OperandType type2(Type::BOOL, {});
  OperandType type3(Type::TENSOR_BOOL8, {2, 3, 2});
  OperandType type7(Type::TENSOR_INT32, {2});
  OperandType type9(Type::TENSOR_BOOL8, {0, 0, 0});
  // Phase 1, operands
  auto input02 = model->addOperand(&type3);
  auto param4 = model->addOperand(&type7);
  auto param5 = model->addOperand(&type2);
  auto output02 = model->addOperand(&type9);
  // Phase 2, operations
  static int32_t param4_init[] = {0, 2};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 2);
  static bool8 param5_init[] = {true};
  model->setOperandValue(param5, param5_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_REDUCE_ALL, {input02, param4, param5}, {output02});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input02},
    {output02});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

