// clang-format off
// Generated file (from: prelu.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2, 2, 3});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 1, 3});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto alpha = model->addOperand(&type1);
  auto output = model->addOperand(&type0);
  // Phase 2, operations
  static float alpha_init[] = {0.0f, 1.0f, 2.0f};
  model->setOperandValue(alpha, alpha_init, sizeof(float) * 3);
  model->addOperation(ANEURALNETWORKS_PRELU, {input, alpha}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input},
    {output});
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2, 2, 3});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 1, 3});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto alpha = model->addOperand(&type1);
  auto output = model->addOperand(&type0);
  // Phase 2, operations
  static float alpha_init[] = {0.0f, 1.0f, 2.0f};
  model->setOperandValue(alpha, alpha_init, sizeof(float) * 3);
  model->addOperation(ANEURALNETWORKS_PRELU, {input, alpha}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input},
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
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3}, 0.25f, 50);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 3}, 0.25f, 128);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 3}, 0.5f, 120);
  // Phase 1, operands
  auto input = model->addOperand(&type3);
  auto alpha = model->addOperand(&type2);
  auto output = model->addOperand(&type4);
  // Phase 2, operations
  static uint8_t alpha_init[] = {50, 54, 58};
  model->setOperandValue(alpha, alpha_init, sizeof(uint8_t) * 3);
  model->addOperation(ANEURALNETWORKS_PRELU, {input, alpha}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant8_2(Model *model) {
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3}, 0.25f, 50);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 3}, 0.25f, 128);
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 3}, 0.25f, 120);
  // Phase 1, operands
  auto input = model->addOperand(&type3);
  auto alpha = model->addOperand(&type2);
  auto output = model->addOperand(&type5);
  // Phase 2, operations
  static uint8_t alpha_init[] = {50, 54, 58};
  model->setOperandValue(alpha, alpha_init, sizeof(uint8_t) * 3);
  model->addOperation(ANEURALNETWORKS_PRELU, {input, alpha}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant8_3(Model *model) {
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 3}, 0.25f, 128);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3}, 0.5f, 50);
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 3}, 0.125f, 120);
  // Phase 1, operands
  auto input = model->addOperand(&type3);
  auto alpha = model->addOperand(&type6);
  auto output = model->addOperand(&type7);
  // Phase 2, operations
  static uint8_t alpha_init[] = {50, 52, 54};
  model->setOperandValue(alpha, alpha_init, sizeof(uint8_t) * 3);
  model->addOperation(ANEURALNETWORKS_PRELU, {input, alpha}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_quant8_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant8_4(Model *model) {
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 3}, 0.25f, 128);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3}, 0.5f, 50);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 3}, 0.1f, 120);
  // Phase 1, operands
  auto input = model->addOperand(&type3);
  auto alpha = model->addOperand(&type6);
  auto output = model->addOperand(&type8);
  // Phase 2, operations
  static uint8_t alpha_init[] = {50, 52, 54};
  model->setOperandValue(alpha, alpha_init, sizeof(uint8_t) * 3);
  model->addOperation(ANEURALNETWORKS_PRELU, {input, alpha}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_quant8_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16(Model *model) {
  OperandType type10(Type::TENSOR_FLOAT16, {1, 2, 2, 3});
  OperandType type9(Type::TENSOR_FLOAT16, {1, 1, 3});
  // Phase 1, operands
  auto input = model->addOperand(&type10);
  auto alpha = model->addOperand(&type9);
  auto output = model->addOperand(&type10);
  // Phase 2, operations
  static _Float16 alpha_init[] = {0.0f, 1.0f, 2.0f};
  model->setOperandValue(alpha, alpha_init, sizeof(_Float16) * 3);
  model->addOperation(ANEURALNETWORKS_PRELU, {input, alpha}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_weight_as_input(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2, 2, 3});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 1, 3});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto alpha = model->addOperand(&type1);
  auto output = model->addOperand(&type0);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_PRELU, {input, alpha}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, alpha},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_weight_as_input_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2, 2, 3});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 1, 3});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto alpha = model->addOperand(&type1);
  auto output = model->addOperand(&type0);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_PRELU, {input, alpha}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, alpha},
    {output});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_weight_as_input_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_weight_as_input_quant8(Model *model) {
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3}, 0.25f, 50);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 3}, 0.25f, 128);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 3}, 0.5f, 120);
  // Phase 1, operands
  auto input = model->addOperand(&type3);
  auto alpha = model->addOperand(&type2);
  auto output = model->addOperand(&type4);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_PRELU, {input, alpha}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, alpha},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_weight_as_input_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_weight_as_input_quant8_2(Model *model) {
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3}, 0.25f, 50);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 3}, 0.25f, 128);
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 3}, 0.25f, 120);
  // Phase 1, operands
  auto input = model->addOperand(&type3);
  auto alpha = model->addOperand(&type2);
  auto output = model->addOperand(&type5);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_PRELU, {input, alpha}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, alpha},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_weight_as_input_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_weight_as_input_quant8_3(Model *model) {
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 3}, 0.25f, 128);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3}, 0.5f, 50);
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 3}, 0.125f, 120);
  // Phase 1, operands
  auto input = model->addOperand(&type3);
  auto alpha = model->addOperand(&type6);
  auto output = model->addOperand(&type7);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_PRELU, {input, alpha}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, alpha},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_weight_as_input_quant8_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_weight_as_input_quant8_4(Model *model) {
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 3}, 0.25f, 128);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3}, 0.5f, 50);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 3}, 0.1f, 120);
  // Phase 1, operands
  auto input = model->addOperand(&type3);
  auto alpha = model->addOperand(&type6);
  auto output = model->addOperand(&type8);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_PRELU, {input, alpha}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, alpha},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_weight_as_input_quant8_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_weight_as_input_float16(Model *model) {
  OperandType type10(Type::TENSOR_FLOAT16, {1, 2, 2, 3});
  OperandType type9(Type::TENSOR_FLOAT16, {1, 1, 3});
  // Phase 1, operands
  auto input = model->addOperand(&type10);
  auto alpha = model->addOperand(&type9);
  auto output = model->addOperand(&type10);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_PRELU, {input, alpha}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, alpha},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_weight_as_input_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2, 2, 3});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 1, 3});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto alpha = model->addOperand(&type1);
  auto output = model->addOperand(&type11);
  // Phase 2, operations
  static float alpha_init[] = {0.0f, 1.0f, 2.0f};
  model->setOperandValue(alpha, alpha_init, sizeof(float) * 3);
  model->addOperation(ANEURALNETWORKS_PRELU, {input, alpha}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2, 2, 3});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 1, 3});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto alpha = model->addOperand(&type1);
  auto output = model->addOperand(&type11);
  // Phase 2, operations
  static float alpha_init[] = {0.0f, 1.0f, 2.0f};
  model->setOperandValue(alpha, alpha_init, sizeof(float) * 3);
  model->addOperation(ANEURALNETWORKS_PRELU, {input, alpha}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input},
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
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 120);
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3}, 0.25f, 50);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 3}, 0.25f, 128);
  // Phase 1, operands
  auto input = model->addOperand(&type3);
  auto alpha = model->addOperand(&type2);
  auto output = model->addOperand(&type12);
  // Phase 2, operations
  static uint8_t alpha_init[] = {50, 54, 58};
  model->setOperandValue(alpha, alpha_init, sizeof(uint8_t) * 3);
  model->addOperation(ANEURALNETWORKS_PRELU, {input, alpha}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_quant8_2(Model *model) {
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 120);
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3}, 0.25f, 50);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 3}, 0.25f, 128);
  // Phase 1, operands
  auto input = model->addOperand(&type3);
  auto alpha = model->addOperand(&type2);
  auto output = model->addOperand(&type13);
  // Phase 2, operations
  static uint8_t alpha_init[] = {50, 54, 58};
  model->setOperandValue(alpha, alpha_init, sizeof(uint8_t) * 3);
  model->addOperation(ANEURALNETWORKS_PRELU, {input, alpha}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_quant8_3(Model *model) {
  OperandType type14(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.125f, 120);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 3}, 0.25f, 128);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3}, 0.5f, 50);
  // Phase 1, operands
  auto input = model->addOperand(&type3);
  auto alpha = model->addOperand(&type6);
  auto output = model->addOperand(&type14);
  // Phase 2, operations
  static uint8_t alpha_init[] = {50, 52, 54};
  model->setOperandValue(alpha, alpha_init, sizeof(uint8_t) * 3);
  model->addOperation(ANEURALNETWORKS_PRELU, {input, alpha}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_quant8_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_quant8_4(Model *model) {
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 120);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 3}, 0.25f, 128);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3}, 0.5f, 50);
  // Phase 1, operands
  auto input = model->addOperand(&type3);
  auto alpha = model->addOperand(&type6);
  auto output = model->addOperand(&type15);
  // Phase 2, operations
  static uint8_t alpha_init[] = {50, 52, 54};
  model->setOperandValue(alpha, alpha_init, sizeof(uint8_t) * 3);
  model->addOperation(ANEURALNETWORKS_PRELU, {input, alpha}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_quant8_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16(Model *model) {
  OperandType type10(Type::TENSOR_FLOAT16, {1, 2, 2, 3});
  OperandType type16(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type9(Type::TENSOR_FLOAT16, {1, 1, 3});
  // Phase 1, operands
  auto input = model->addOperand(&type10);
  auto alpha = model->addOperand(&type9);
  auto output = model->addOperand(&type16);
  // Phase 2, operations
  static _Float16 alpha_init[] = {0.0f, 1.0f, 2.0f};
  model->setOperandValue(alpha, alpha_init, sizeof(_Float16) * 3);
  model->addOperation(ANEURALNETWORKS_PRELU, {input, alpha}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_weight_as_input(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2, 2, 3});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 1, 3});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto alpha = model->addOperand(&type1);
  auto output = model->addOperand(&type11);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_PRELU, {input, alpha}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, alpha},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_weight_as_input_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2, 2, 3});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 1, 3});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto alpha = model->addOperand(&type1);
  auto output = model->addOperand(&type11);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_PRELU, {input, alpha}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, alpha},
    {output});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_weight_as_input_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_weight_as_input_quant8(Model *model) {
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 120);
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3}, 0.25f, 50);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 3}, 0.25f, 128);
  // Phase 1, operands
  auto input = model->addOperand(&type3);
  auto alpha = model->addOperand(&type2);
  auto output = model->addOperand(&type12);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_PRELU, {input, alpha}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, alpha},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_weight_as_input_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_weight_as_input_quant8_2(Model *model) {
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 120);
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3}, 0.25f, 50);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 3}, 0.25f, 128);
  // Phase 1, operands
  auto input = model->addOperand(&type3);
  auto alpha = model->addOperand(&type2);
  auto output = model->addOperand(&type13);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_PRELU, {input, alpha}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, alpha},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_weight_as_input_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_weight_as_input_quant8_3(Model *model) {
  OperandType type14(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.125f, 120);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 3}, 0.25f, 128);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3}, 0.5f, 50);
  // Phase 1, operands
  auto input = model->addOperand(&type3);
  auto alpha = model->addOperand(&type6);
  auto output = model->addOperand(&type14);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_PRELU, {input, alpha}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, alpha},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_weight_as_input_quant8_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_weight_as_input_quant8_4(Model *model) {
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 120);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 3}, 0.25f, 128);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3}, 0.5f, 50);
  // Phase 1, operands
  auto input = model->addOperand(&type3);
  auto alpha = model->addOperand(&type6);
  auto output = model->addOperand(&type15);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_PRELU, {input, alpha}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, alpha},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_weight_as_input_quant8_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_weight_as_input_float16(Model *model) {
  OperandType type10(Type::TENSOR_FLOAT16, {1, 2, 2, 3});
  OperandType type16(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type9(Type::TENSOR_FLOAT16, {1, 1, 3});
  // Phase 1, operands
  auto input = model->addOperand(&type10);
  auto alpha = model->addOperand(&type9);
  auto output = model->addOperand(&type16);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_PRELU, {input, alpha}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, alpha},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_weight_as_input_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

