// clang-format off
// Generated file (from: concat_mixed_quant.mod.py). Do not edit
void CreateModel_quant8(Model *model) {
  OperandType type2(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {2, 1, 2}, 0.084f, 127);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {2, 1, 2}, 0.05f, 0);
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {2, 1, 2}, 0.089f, 123);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {2, 1, 2}, 0.029f, 0);
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {2, 1, 8}, 0.1f, 127);
  // Phase 1, operands
  auto input0 = model->addOperand(&type3);
  auto input1 = model->addOperand(&type4);
  auto input2 = model->addOperand(&type5);
  auto input3 = model->addOperand(&type6);
  auto param = model->addOperand(&type2);
  auto output0 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t param_init[] = {2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONCATENATION, {input0, input1, input2, input3, param}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1, input2, input3},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_quant8(Model *model) {
  OperandType type2(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {2, 1, 2}, 0.084f, 127);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {2, 1, 2}, 0.05f, 0);
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {2, 1, 2}, 0.089f, 123);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {2, 1, 2}, 0.029f, 0);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0}, 0.1f, 127);
  // Phase 1, operands
  auto input0 = model->addOperand(&type3);
  auto input1 = model->addOperand(&type4);
  auto input2 = model->addOperand(&type5);
  auto input3 = model->addOperand(&type6);
  auto param = model->addOperand(&type2);
  auto output0 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param_init[] = {2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONCATENATION, {input0, input1, input2, input3, param}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1, input2, input3},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant8_2(Model *model) {
  OperandType type2(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {2, 1, 2}, 0.084f, 127);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {2, 1, 2}, 0.05f, 0);
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {2, 1, 2}, 0.089f, 123);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {2, 1, 2}, 0.029f, 0);
  OperandType type9(Type::TENSOR_QUANT8_ASYMM, {2, 1, 8}, 0.0078125f, 127);
  // Phase 1, operands
  auto input0 = model->addOperand(&type3);
  auto input1 = model->addOperand(&type4);
  auto input2 = model->addOperand(&type5);
  auto input3 = model->addOperand(&type6);
  auto param = model->addOperand(&type2);
  auto output0 = model->addOperand(&type9);
  // Phase 2, operations
  static int32_t param_init[] = {2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONCATENATION, {input0, input1, input2, input3, param}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1, input2, input3},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_quant8_2(Model *model) {
  OperandType type10(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0}, 0.0078125f, 127);
  OperandType type2(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {2, 1, 2}, 0.084f, 127);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {2, 1, 2}, 0.05f, 0);
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {2, 1, 2}, 0.089f, 123);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {2, 1, 2}, 0.029f, 0);
  // Phase 1, operands
  auto input0 = model->addOperand(&type3);
  auto input1 = model->addOperand(&type4);
  auto input2 = model->addOperand(&type5);
  auto input3 = model->addOperand(&type6);
  auto param = model->addOperand(&type2);
  auto output0 = model->addOperand(&type10);
  // Phase 2, operations
  static int32_t param_init[] = {2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONCATENATION, {input0, input1, input2, input3, param}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1, input2, input3},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

