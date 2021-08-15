// clang-format off
// Generated file (from: sub_v1_2_broadcast.mod.py). Do not edit
void CreateModel_none(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type1(Type::TENSOR_FLOAT32, {2, 2});
  OperandType type2(Type::INT32, {});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto input1 = model->addOperand(&type1);
  auto act = model->addOperand(&type2);
  auto output0 = model->addOperand(&type1);
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
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type1(Type::TENSOR_FLOAT32, {2, 2});
  OperandType type2(Type::INT32, {});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto input1 = model->addOperand(&type1);
  auto act = model->addOperand(&type2);
  auto output0 = model->addOperand(&type1);
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
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type1(Type::TENSOR_FLOAT32, {2, 2});
  OperandType type2(Type::INT32, {});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto input1 = model->addOperand(&type1);
  auto act = model->addOperand(&type2);
  auto output0 = model->addOperand(&type1);
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
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type1(Type::TENSOR_FLOAT32, {2, 2});
  OperandType type2(Type::INT32, {});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto input1 = model->addOperand(&type1);
  auto act = model->addOperand(&type2);
  auto output0 = model->addOperand(&type1);
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
  OperandType type2(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type6(Type::TENSOR_FLOAT16, {2, 2});
  // Phase 1, operands
  auto input0 = model->addOperand(&type5);
  auto input1 = model->addOperand(&type6);
  auto act = model->addOperand(&type2);
  auto output0 = model->addOperand(&type6);
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
  OperandType type2(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type6(Type::TENSOR_FLOAT16, {2, 2});
  // Phase 1, operands
  auto input0 = model->addOperand(&type5);
  auto input1 = model->addOperand(&type6);
  auto act = model->addOperand(&type2);
  auto output0 = model->addOperand(&type6);
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
  OperandType type2(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type6(Type::TENSOR_FLOAT16, {2, 2});
  // Phase 1, operands
  auto input0 = model->addOperand(&type5);
  auto input1 = model->addOperand(&type6);
  auto act = model->addOperand(&type2);
  auto output0 = model->addOperand(&type6);
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
  OperandType type2(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type6(Type::TENSOR_FLOAT16, {2, 2});
  // Phase 1, operands
  auto input0 = model->addOperand(&type5);
  auto input1 = model->addOperand(&type6);
  auto act = model->addOperand(&type2);
  auto output0 = model->addOperand(&type6);
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
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type1(Type::TENSOR_FLOAT32, {2, 2});
  OperandType type2(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto input1 = model->addOperand(&type1);
  auto act = model->addOperand(&type2);
  auto output0 = model->addOperand(&type7);
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
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type1(Type::TENSOR_FLOAT32, {2, 2});
  OperandType type2(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto input1 = model->addOperand(&type1);
  auto act = model->addOperand(&type2);
  auto output0 = model->addOperand(&type7);
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
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type1(Type::TENSOR_FLOAT32, {2, 2});
  OperandType type2(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto input1 = model->addOperand(&type1);
  auto act = model->addOperand(&type2);
  auto output0 = model->addOperand(&type7);
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
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type1(Type::TENSOR_FLOAT32, {2, 2});
  OperandType type2(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto input1 = model->addOperand(&type1);
  auto act = model->addOperand(&type2);
  auto output0 = model->addOperand(&type7);
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
  OperandType type2(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type6(Type::TENSOR_FLOAT16, {2, 2});
  OperandType type8(Type::TENSOR_FLOAT16, {0, 0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type5);
  auto input1 = model->addOperand(&type6);
  auto act = model->addOperand(&type2);
  auto output0 = model->addOperand(&type8);
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
  OperandType type2(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type6(Type::TENSOR_FLOAT16, {2, 2});
  OperandType type8(Type::TENSOR_FLOAT16, {0, 0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type5);
  auto input1 = model->addOperand(&type6);
  auto act = model->addOperand(&type2);
  auto output0 = model->addOperand(&type8);
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
  OperandType type2(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type6(Type::TENSOR_FLOAT16, {2, 2});
  OperandType type8(Type::TENSOR_FLOAT16, {0, 0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type5);
  auto input1 = model->addOperand(&type6);
  auto act = model->addOperand(&type2);
  auto output0 = model->addOperand(&type8);
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
  OperandType type2(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type6(Type::TENSOR_FLOAT16, {2, 2});
  OperandType type8(Type::TENSOR_FLOAT16, {0, 0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type5);
  auto input1 = model->addOperand(&type6);
  auto act = model->addOperand(&type2);
  auto output0 = model->addOperand(&type8);
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
  OperandType type2(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 1.0f, 0);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {2, 2}, 1.0f, 0);
  // Phase 1, operands
  auto input01 = model->addOperand(&type3);
  auto input11 = model->addOperand(&type4);
  auto param = model->addOperand(&type2);
  auto output01 = model->addOperand(&type4);
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
  OperandType type2(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 1.0f, 0);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {2, 2}, 1.0f, 0);
  OperandType type9(Type::TENSOR_QUANT8_ASYMM, {0, 0}, 1.0f, 0);
  // Phase 1, operands
  auto input01 = model->addOperand(&type3);
  auto input11 = model->addOperand(&type4);
  auto param = model->addOperand(&type2);
  auto output01 = model->addOperand(&type9);
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

