// clang-format off
// Generated file (from: unidirectional_sequence_rnn.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 16, 8});
  OperandType type1(Type::TENSOR_FLOAT32, {16, 8});
  OperandType type2(Type::TENSOR_FLOAT32, {16, 16});
  OperandType type3(Type::TENSOR_FLOAT32, {16});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 16});
  OperandType type5(Type::TENSOR_FLOAT32, {2, 16, 16});
  OperandType type6(Type::INT32, {});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto weights = model->addOperand(&type1);
  auto recurrent_weights = model->addOperand(&type2);
  auto bias = model->addOperand(&type3);
  auto hidden_state = model->addOperand(&type4);
  auto activation = model->addOperand(&type6);
  auto time_major = model->addOperand(&type6);
  auto output = model->addOperand(&type5);
  // Phase 2, operations
  static int32_t activation_init[] = {1};
  model->setOperandValue(activation, activation_init, sizeof(int32_t) * 1);
  static int32_t time_major_init[] = {0};
  model->setOperandValue(time_major, time_major_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_UNIDIRECTIONAL_SEQUENCE_RNN, {input, weights, recurrent_weights, bias, hidden_state, activation, time_major}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, weights, recurrent_weights, bias, hidden_state},
    {output});
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 16, 8});
  OperandType type1(Type::TENSOR_FLOAT32, {16, 8});
  OperandType type2(Type::TENSOR_FLOAT32, {16, 16});
  OperandType type3(Type::TENSOR_FLOAT32, {16});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 16});
  OperandType type5(Type::TENSOR_FLOAT32, {2, 16, 16});
  OperandType type6(Type::INT32, {});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto weights = model->addOperand(&type1);
  auto recurrent_weights = model->addOperand(&type2);
  auto bias = model->addOperand(&type3);
  auto hidden_state = model->addOperand(&type4);
  auto activation = model->addOperand(&type6);
  auto time_major = model->addOperand(&type6);
  auto output = model->addOperand(&type5);
  // Phase 2, operations
  static int32_t activation_init[] = {1};
  model->setOperandValue(activation, activation_init, sizeof(int32_t) * 1);
  static int32_t time_major_init[] = {0};
  model->setOperandValue(time_major, time_major_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_UNIDIRECTIONAL_SEQUENCE_RNN, {input, weights, recurrent_weights, bias, hidden_state, activation, time_major}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, weights, recurrent_weights, bias, hidden_state},
    {output});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16(Model *model) {
  OperandType type10(Type::TENSOR_FLOAT16, {2, 16});
  OperandType type11(Type::TENSOR_FLOAT16, {2, 16, 8});
  OperandType type12(Type::TENSOR_FLOAT16, {2, 16, 16});
  OperandType type13(Type::TENSOR_FLOAT16, {16, 16});
  OperandType type14(Type::TENSOR_FLOAT16, {16, 8});
  OperandType type6(Type::INT32, {});
  OperandType type9(Type::TENSOR_FLOAT16, {16});
  // Phase 1, operands
  auto input = model->addOperand(&type11);
  auto weights = model->addOperand(&type14);
  auto recurrent_weights = model->addOperand(&type13);
  auto bias = model->addOperand(&type9);
  auto hidden_state = model->addOperand(&type10);
  auto activation = model->addOperand(&type6);
  auto time_major = model->addOperand(&type6);
  auto output = model->addOperand(&type12);
  // Phase 2, operations
  static int32_t activation_init[] = {1};
  model->setOperandValue(activation, activation_init, sizeof(int32_t) * 1);
  static int32_t time_major_init[] = {0};
  model->setOperandValue(time_major, time_major_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_UNIDIRECTIONAL_SEQUENCE_RNN, {input, weights, recurrent_weights, bias, hidden_state, activation, time_major}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, weights, recurrent_weights, bias, hidden_state},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 16, 8});
  OperandType type1(Type::TENSOR_FLOAT32, {16, 8});
  OperandType type15(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {16, 16});
  OperandType type3(Type::TENSOR_FLOAT32, {16});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 16});
  OperandType type6(Type::INT32, {});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto weights = model->addOperand(&type1);
  auto recurrent_weights = model->addOperand(&type2);
  auto bias = model->addOperand(&type3);
  auto hidden_state = model->addOperand(&type4);
  auto activation = model->addOperand(&type6);
  auto time_major = model->addOperand(&type6);
  auto output = model->addOperand(&type15);
  // Phase 2, operations
  static int32_t activation_init[] = {1};
  model->setOperandValue(activation, activation_init, sizeof(int32_t) * 1);
  static int32_t time_major_init[] = {0};
  model->setOperandValue(time_major, time_major_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_UNIDIRECTIONAL_SEQUENCE_RNN, {input, weights, recurrent_weights, bias, hidden_state, activation, time_major}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, weights, recurrent_weights, bias, hidden_state},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 16, 8});
  OperandType type1(Type::TENSOR_FLOAT32, {16, 8});
  OperandType type15(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {16, 16});
  OperandType type3(Type::TENSOR_FLOAT32, {16});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 16});
  OperandType type6(Type::INT32, {});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto weights = model->addOperand(&type1);
  auto recurrent_weights = model->addOperand(&type2);
  auto bias = model->addOperand(&type3);
  auto hidden_state = model->addOperand(&type4);
  auto activation = model->addOperand(&type6);
  auto time_major = model->addOperand(&type6);
  auto output = model->addOperand(&type15);
  // Phase 2, operations
  static int32_t activation_init[] = {1};
  model->setOperandValue(activation, activation_init, sizeof(int32_t) * 1);
  static int32_t time_major_init[] = {0};
  model->setOperandValue(time_major, time_major_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_UNIDIRECTIONAL_SEQUENCE_RNN, {input, weights, recurrent_weights, bias, hidden_state, activation, time_major}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, weights, recurrent_weights, bias, hidden_state},
    {output});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16(Model *model) {
  OperandType type10(Type::TENSOR_FLOAT16, {2, 16});
  OperandType type11(Type::TENSOR_FLOAT16, {2, 16, 8});
  OperandType type13(Type::TENSOR_FLOAT16, {16, 16});
  OperandType type14(Type::TENSOR_FLOAT16, {16, 8});
  OperandType type16(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type6(Type::INT32, {});
  OperandType type9(Type::TENSOR_FLOAT16, {16});
  // Phase 1, operands
  auto input = model->addOperand(&type11);
  auto weights = model->addOperand(&type14);
  auto recurrent_weights = model->addOperand(&type13);
  auto bias = model->addOperand(&type9);
  auto hidden_state = model->addOperand(&type10);
  auto activation = model->addOperand(&type6);
  auto time_major = model->addOperand(&type6);
  auto output = model->addOperand(&type16);
  // Phase 2, operations
  static int32_t activation_init[] = {1};
  model->setOperandValue(activation, activation_init, sizeof(int32_t) * 1);
  static int32_t time_major_init[] = {0};
  model->setOperandValue(time_major, time_major_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_UNIDIRECTIONAL_SEQUENCE_RNN, {input, weights, recurrent_weights, bias, hidden_state, activation, time_major}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, weights, recurrent_weights, bias, hidden_state},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_2(Model *model) {
  OperandType type1(Type::TENSOR_FLOAT32, {16, 8});
  OperandType type2(Type::TENSOR_FLOAT32, {16, 16});
  OperandType type3(Type::TENSOR_FLOAT32, {16});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 16});
  OperandType type6(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {16, 2, 8});
  OperandType type8(Type::TENSOR_FLOAT32, {16, 2, 16});
  // Phase 1, operands
  auto input1 = model->addOperand(&type7);
  auto weights1 = model->addOperand(&type1);
  auto recurrent_weights1 = model->addOperand(&type2);
  auto bias1 = model->addOperand(&type3);
  auto hidden_state1 = model->addOperand(&type4);
  auto activation1 = model->addOperand(&type6);
  auto time_major1 = model->addOperand(&type6);
  auto output1 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t activation1_init[] = {1};
  model->setOperandValue(activation1, activation1_init, sizeof(int32_t) * 1);
  static int32_t time_major1_init[] = {1};
  model->setOperandValue(time_major1, time_major1_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_UNIDIRECTIONAL_SEQUENCE_RNN, {input1, weights1, recurrent_weights1, bias1, hidden_state1, activation1, time_major1}, {output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input1, weights1, recurrent_weights1, bias1, hidden_state1},
    {output1});
  assert(model->isValid());
}

inline bool is_ignored_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_2(Model *model) {
  OperandType type1(Type::TENSOR_FLOAT32, {16, 8});
  OperandType type2(Type::TENSOR_FLOAT32, {16, 16});
  OperandType type3(Type::TENSOR_FLOAT32, {16});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 16});
  OperandType type6(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {16, 2, 8});
  OperandType type8(Type::TENSOR_FLOAT32, {16, 2, 16});
  // Phase 1, operands
  auto input1 = model->addOperand(&type7);
  auto weights1 = model->addOperand(&type1);
  auto recurrent_weights1 = model->addOperand(&type2);
  auto bias1 = model->addOperand(&type3);
  auto hidden_state1 = model->addOperand(&type4);
  auto activation1 = model->addOperand(&type6);
  auto time_major1 = model->addOperand(&type6);
  auto output1 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t activation1_init[] = {1};
  model->setOperandValue(activation1, activation1_init, sizeof(int32_t) * 1);
  static int32_t time_major1_init[] = {1};
  model->setOperandValue(time_major1, time_major1_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_UNIDIRECTIONAL_SEQUENCE_RNN, {input1, weights1, recurrent_weights1, bias1, hidden_state1, activation1, time_major1}, {output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input1, weights1, recurrent_weights1, bias1, hidden_state1},
    {output1});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_2(Model *model) {
  OperandType type10(Type::TENSOR_FLOAT16, {2, 16});
  OperandType type13(Type::TENSOR_FLOAT16, {16, 16});
  OperandType type14(Type::TENSOR_FLOAT16, {16, 8});
  OperandType type17(Type::TENSOR_FLOAT16, {16, 2, 8});
  OperandType type18(Type::TENSOR_FLOAT16, {16, 2, 16});
  OperandType type6(Type::INT32, {});
  OperandType type9(Type::TENSOR_FLOAT16, {16});
  // Phase 1, operands
  auto input1 = model->addOperand(&type17);
  auto weights1 = model->addOperand(&type14);
  auto recurrent_weights1 = model->addOperand(&type13);
  auto bias1 = model->addOperand(&type9);
  auto hidden_state1 = model->addOperand(&type10);
  auto activation1 = model->addOperand(&type6);
  auto time_major1 = model->addOperand(&type6);
  auto output1 = model->addOperand(&type18);
  // Phase 2, operations
  static int32_t activation1_init[] = {1};
  model->setOperandValue(activation1, activation1_init, sizeof(int32_t) * 1);
  static int32_t time_major1_init[] = {1};
  model->setOperandValue(time_major1, time_major1_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_UNIDIRECTIONAL_SEQUENCE_RNN, {input1, weights1, recurrent_weights1, bias1, hidden_state1, activation1, time_major1}, {output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input1, weights1, recurrent_weights1, bias1, hidden_state1},
    {output1});
  assert(model->isValid());
}

inline bool is_ignored_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_2(Model *model) {
  OperandType type1(Type::TENSOR_FLOAT32, {16, 8});
  OperandType type15(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {16, 16});
  OperandType type3(Type::TENSOR_FLOAT32, {16});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 16});
  OperandType type6(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {16, 2, 8});
  // Phase 1, operands
  auto input1 = model->addOperand(&type7);
  auto weights1 = model->addOperand(&type1);
  auto recurrent_weights1 = model->addOperand(&type2);
  auto bias1 = model->addOperand(&type3);
  auto hidden_state1 = model->addOperand(&type4);
  auto activation1 = model->addOperand(&type6);
  auto time_major1 = model->addOperand(&type6);
  auto output1 = model->addOperand(&type15);
  // Phase 2, operations
  static int32_t activation1_init[] = {1};
  model->setOperandValue(activation1, activation1_init, sizeof(int32_t) * 1);
  static int32_t time_major1_init[] = {1};
  model->setOperandValue(time_major1, time_major1_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_UNIDIRECTIONAL_SEQUENCE_RNN, {input1, weights1, recurrent_weights1, bias1, hidden_state1, activation1, time_major1}, {output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input1, weights1, recurrent_weights1, bias1, hidden_state1},
    {output1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_2(Model *model) {
  OperandType type1(Type::TENSOR_FLOAT32, {16, 8});
  OperandType type15(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {16, 16});
  OperandType type3(Type::TENSOR_FLOAT32, {16});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 16});
  OperandType type6(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {16, 2, 8});
  // Phase 1, operands
  auto input1 = model->addOperand(&type7);
  auto weights1 = model->addOperand(&type1);
  auto recurrent_weights1 = model->addOperand(&type2);
  auto bias1 = model->addOperand(&type3);
  auto hidden_state1 = model->addOperand(&type4);
  auto activation1 = model->addOperand(&type6);
  auto time_major1 = model->addOperand(&type6);
  auto output1 = model->addOperand(&type15);
  // Phase 2, operations
  static int32_t activation1_init[] = {1};
  model->setOperandValue(activation1, activation1_init, sizeof(int32_t) * 1);
  static int32_t time_major1_init[] = {1};
  model->setOperandValue(time_major1, time_major1_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_UNIDIRECTIONAL_SEQUENCE_RNN, {input1, weights1, recurrent_weights1, bias1, hidden_state1, activation1, time_major1}, {output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input1, weights1, recurrent_weights1, bias1, hidden_state1},
    {output1});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_2(Model *model) {
  OperandType type10(Type::TENSOR_FLOAT16, {2, 16});
  OperandType type13(Type::TENSOR_FLOAT16, {16, 16});
  OperandType type14(Type::TENSOR_FLOAT16, {16, 8});
  OperandType type16(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type17(Type::TENSOR_FLOAT16, {16, 2, 8});
  OperandType type6(Type::INT32, {});
  OperandType type9(Type::TENSOR_FLOAT16, {16});
  // Phase 1, operands
  auto input1 = model->addOperand(&type17);
  auto weights1 = model->addOperand(&type14);
  auto recurrent_weights1 = model->addOperand(&type13);
  auto bias1 = model->addOperand(&type9);
  auto hidden_state1 = model->addOperand(&type10);
  auto activation1 = model->addOperand(&type6);
  auto time_major1 = model->addOperand(&type6);
  auto output1 = model->addOperand(&type16);
  // Phase 2, operations
  static int32_t activation1_init[] = {1};
  model->setOperandValue(activation1, activation1_init, sizeof(int32_t) * 1);
  static int32_t time_major1_init[] = {1};
  model->setOperandValue(time_major1, time_major1_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_UNIDIRECTIONAL_SEQUENCE_RNN, {input1, weights1, recurrent_weights1, bias1, hidden_state1, activation1, time_major1}, {output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input1, weights1, recurrent_weights1, bias1, hidden_state1},
    {output1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

