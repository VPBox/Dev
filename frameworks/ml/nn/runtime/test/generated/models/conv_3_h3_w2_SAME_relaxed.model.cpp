// clang-format off
// Generated file (from: conv_3_h3_w2_SAME_relaxed.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::INT32, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 8, 8, 3});
  OperandType type2(Type::TENSOR_FLOAT32, {3, 3, 2, 3});
  OperandType type3(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op2 = model->addOperand(&type1);
  auto op0 = model->addOperand(&type2);
  auto op1 = model->addOperand(&type3);
  auto b4 = model->addOperand(&type0);
  auto b5 = model->addOperand(&type0);
  auto b6 = model->addOperand(&type0);
  auto b7 = model->addOperand(&type0);
  auto op3 = model->addOperand(&type1);
  // Phase 2, operations
  static float op0_init[] = {-0.966213f, -0.579455f, -0.684259f, 0.738216f, 0.184325f, 0.0973683f, -0.176863f, -0.23936f, -0.000233404f, 0.055546f, -0.232658f, -0.316404f, -0.012904f, 0.320705f, -0.326657f, -0.919674f, 0.868081f, -0.824608f, -0.467474f, 0.0278809f, 0.563238f, 0.386045f, -0.270568f, -0.941308f, -0.779227f, -0.261492f, -0.774804f, -0.79665f, 0.22473f, -0.414312f, 0.685897f, -0.327792f, 0.77395f, -0.714578f, -0.972365f, 0.0696099f, -0.82203f, -0.79946f, 0.37289f, -0.917775f, 0.82236f, -0.144706f, -0.167188f, 0.268062f, 0.702641f, -0.412223f, 0.755759f, 0.721547f, -0.43637f, -0.274905f, -0.269165f, 0.16102f, 0.819857f, -0.312008f};
  model->setOperandValue(op0, op0_init, sizeof(float) * 54);
  static float op1_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op1, op1_init, sizeof(float) * 3);
  static int32_t b4_init[] = {1};
  model->setOperandValue(b4, b4_init, sizeof(int32_t) * 1);
  static int32_t b5_init[] = {1};
  model->setOperandValue(b5, b5_init, sizeof(int32_t) * 1);
  static int32_t b6_init[] = {1};
  model->setOperandValue(b6, b6_init, sizeof(int32_t) * 1);
  static int32_t b7_init[] = {0};
  model->setOperandValue(b7, b7_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op2, op0, op1, b4, b5, b6, b7}, {op3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op2},
    {op3});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::INT32, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 8, 8, 3});
  OperandType type2(Type::TENSOR_FLOAT32, {3, 3, 2, 3});
  OperandType type3(Type::TENSOR_FLOAT32, {3});
  OperandType type4(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op2 = model->addOperand(&type1);
  auto op0 = model->addOperand(&type2);
  auto op1 = model->addOperand(&type3);
  auto b4 = model->addOperand(&type0);
  auto b5 = model->addOperand(&type0);
  auto b6 = model->addOperand(&type0);
  auto b7 = model->addOperand(&type0);
  auto op3 = model->addOperand(&type4);
  // Phase 2, operations
  static float op0_init[] = {-0.966213f, -0.579455f, -0.684259f, 0.738216f, 0.184325f, 0.0973683f, -0.176863f, -0.23936f, -0.000233404f, 0.055546f, -0.232658f, -0.316404f, -0.012904f, 0.320705f, -0.326657f, -0.919674f, 0.868081f, -0.824608f, -0.467474f, 0.0278809f, 0.563238f, 0.386045f, -0.270568f, -0.941308f, -0.779227f, -0.261492f, -0.774804f, -0.79665f, 0.22473f, -0.414312f, 0.685897f, -0.327792f, 0.77395f, -0.714578f, -0.972365f, 0.0696099f, -0.82203f, -0.79946f, 0.37289f, -0.917775f, 0.82236f, -0.144706f, -0.167188f, 0.268062f, 0.702641f, -0.412223f, 0.755759f, 0.721547f, -0.43637f, -0.274905f, -0.269165f, 0.16102f, 0.819857f, -0.312008f};
  model->setOperandValue(op0, op0_init, sizeof(float) * 54);
  static float op1_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op1, op1_init, sizeof(float) * 3);
  static int32_t b4_init[] = {1};
  model->setOperandValue(b4, b4_init, sizeof(int32_t) * 1);
  static int32_t b5_init[] = {1};
  model->setOperandValue(b5, b5_init, sizeof(int32_t) * 1);
  static int32_t b6_init[] = {1};
  model->setOperandValue(b6, b6_init, sizeof(int32_t) * 1);
  static int32_t b7_init[] = {0};
  model->setOperandValue(b7, b7_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op2, op0, op1, b4, b5, b6, b7}, {op3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op2},
    {op3});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_2(Model *model) {
  OperandType type0(Type::INT32, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 8, 8, 3});
  OperandType type2(Type::TENSOR_FLOAT32, {3, 3, 2, 3});
  OperandType type3(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op2 = model->addOperand(&type1);
  auto op0 = model->addOperand(&type2);
  auto op1 = model->addOperand(&type3);
  auto b4 = model->addOperand(&type0);
  auto b5 = model->addOperand(&type0);
  auto b6 = model->addOperand(&type0);
  auto b7 = model->addOperand(&type0);
  auto op3 = model->addOperand(&type1);
  // Phase 2, operations
  static float op0_init[] = {-0.966213f, -0.579455f, -0.684259f, 0.738216f, 0.184325f, 0.0973683f, -0.176863f, -0.23936f, -0.000233404f, 0.055546f, -0.232658f, -0.316404f, -0.012904f, 0.320705f, -0.326657f, -0.919674f, 0.868081f, -0.824608f, -0.467474f, 0.0278809f, 0.563238f, 0.386045f, -0.270568f, -0.941308f, -0.779227f, -0.261492f, -0.774804f, -0.79665f, 0.22473f, -0.414312f, 0.685897f, -0.327792f, 0.77395f, -0.714578f, -0.972365f, 0.0696099f, -0.82203f, -0.79946f, 0.37289f, -0.917775f, 0.82236f, -0.144706f, -0.167188f, 0.268062f, 0.702641f, -0.412223f, 0.755759f, 0.721547f, -0.43637f, -0.274905f, -0.269165f, 0.16102f, 0.819857f, -0.312008f};
  model->setOperandValue(op0, op0_init, sizeof(float) * 54);
  static float op1_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op1, op1_init, sizeof(float) * 3);
  static int32_t b4_init[] = {1};
  model->setOperandValue(b4, b4_init, sizeof(int32_t) * 1);
  static int32_t b5_init[] = {1};
  model->setOperandValue(b5, b5_init, sizeof(int32_t) * 1);
  static int32_t b6_init[] = {1};
  model->setOperandValue(b6, b6_init, sizeof(int32_t) * 1);
  static int32_t b7_init[] = {0};
  model->setOperandValue(b7, b7_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op2, op0, op1, b4, b5, b6, b7}, {op3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op2},
    {op3});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_2(Model *model) {
  OperandType type0(Type::INT32, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 8, 8, 3});
  OperandType type2(Type::TENSOR_FLOAT32, {3, 3, 2, 3});
  OperandType type3(Type::TENSOR_FLOAT32, {3});
  OperandType type4(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op2 = model->addOperand(&type1);
  auto op0 = model->addOperand(&type2);
  auto op1 = model->addOperand(&type3);
  auto b4 = model->addOperand(&type0);
  auto b5 = model->addOperand(&type0);
  auto b6 = model->addOperand(&type0);
  auto b7 = model->addOperand(&type0);
  auto op3 = model->addOperand(&type4);
  // Phase 2, operations
  static float op0_init[] = {-0.966213f, -0.579455f, -0.684259f, 0.738216f, 0.184325f, 0.0973683f, -0.176863f, -0.23936f, -0.000233404f, 0.055546f, -0.232658f, -0.316404f, -0.012904f, 0.320705f, -0.326657f, -0.919674f, 0.868081f, -0.824608f, -0.467474f, 0.0278809f, 0.563238f, 0.386045f, -0.270568f, -0.941308f, -0.779227f, -0.261492f, -0.774804f, -0.79665f, 0.22473f, -0.414312f, 0.685897f, -0.327792f, 0.77395f, -0.714578f, -0.972365f, 0.0696099f, -0.82203f, -0.79946f, 0.37289f, -0.917775f, 0.82236f, -0.144706f, -0.167188f, 0.268062f, 0.702641f, -0.412223f, 0.755759f, 0.721547f, -0.43637f, -0.274905f, -0.269165f, 0.16102f, 0.819857f, -0.312008f};
  model->setOperandValue(op0, op0_init, sizeof(float) * 54);
  static float op1_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op1, op1_init, sizeof(float) * 3);
  static int32_t b4_init[] = {1};
  model->setOperandValue(b4, b4_init, sizeof(int32_t) * 1);
  static int32_t b5_init[] = {1};
  model->setOperandValue(b5, b5_init, sizeof(int32_t) * 1);
  static int32_t b6_init[] = {1};
  model->setOperandValue(b6, b6_init, sizeof(int32_t) * 1);
  static int32_t b7_init[] = {0};
  model->setOperandValue(b7, b7_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op2, op0, op1, b4, b5, b6, b7}, {op3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op2},
    {op3});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

