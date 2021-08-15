// clang-format off
// Generated file (from: bidirectional_sequence_rnn.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 16, 8});
  OperandType type1(Type::TENSOR_FLOAT32, {16, 8});
  OperandType type2(Type::TENSOR_FLOAT32, {16, 16});
  OperandType type3(Type::TENSOR_FLOAT32, {16});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 16});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 16, 16});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto fw_weights = model->addOperand(&type1);
  auto fw_recurrent_weights = model->addOperand(&type2);
  auto fw_bias = model->addOperand(&type3);
  auto fw_hidden_state = model->addOperand(&type4);
  auto bw_weights = model->addOperand(&type1);
  auto bw_recurrent_weights = model->addOperand(&type2);
  auto bw_bias = model->addOperand(&type3);
  auto bw_hidden_state = model->addOperand(&type4);
  auto aux_input = model->addOperand(&type5);
  auto fw_aux_weights = model->addOperand(&type5);
  auto bw_aux_weights = model->addOperand(&type5);
  auto activation = model->addOperand(&type7);
  auto time_major = model->addOperand(&type8);
  auto merge_outputs = model->addOperand(&type8);
  auto fw_output = model->addOperand(&type6);
  auto bw_output = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t activation_init[] = {1};
  model->setOperandValue(activation, activation_init, sizeof(int32_t) * 1);
  static bool8 time_major_init[] = {false};
  model->setOperandValue(time_major, time_major_init, sizeof(bool8) * 1);
  static bool8 merge_outputs_init[] = {false};
  model->setOperandValue(merge_outputs, merge_outputs_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input, fw_weights, fw_recurrent_weights, fw_bias, fw_hidden_state, bw_weights, bw_recurrent_weights, bw_bias, bw_hidden_state, aux_input, fw_aux_weights, bw_aux_weights, activation, time_major, merge_outputs}, {fw_output, bw_output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, fw_weights, fw_recurrent_weights, fw_bias, fw_hidden_state, bw_weights, bw_recurrent_weights, bw_bias, bw_hidden_state, aux_input, fw_aux_weights, bw_aux_weights},
    {fw_output, bw_output});
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
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 16, 16});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto fw_weights = model->addOperand(&type1);
  auto fw_recurrent_weights = model->addOperand(&type2);
  auto fw_bias = model->addOperand(&type3);
  auto fw_hidden_state = model->addOperand(&type4);
  auto bw_weights = model->addOperand(&type1);
  auto bw_recurrent_weights = model->addOperand(&type2);
  auto bw_bias = model->addOperand(&type3);
  auto bw_hidden_state = model->addOperand(&type4);
  auto aux_input = model->addOperand(&type5);
  auto fw_aux_weights = model->addOperand(&type5);
  auto bw_aux_weights = model->addOperand(&type5);
  auto activation = model->addOperand(&type7);
  auto time_major = model->addOperand(&type8);
  auto merge_outputs = model->addOperand(&type8);
  auto fw_output = model->addOperand(&type6);
  auto bw_output = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t activation_init[] = {1};
  model->setOperandValue(activation, activation_init, sizeof(int32_t) * 1);
  static bool8 time_major_init[] = {false};
  model->setOperandValue(time_major, time_major_init, sizeof(bool8) * 1);
  static bool8 merge_outputs_init[] = {false};
  model->setOperandValue(merge_outputs, merge_outputs_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input, fw_weights, fw_recurrent_weights, fw_bias, fw_hidden_state, bw_weights, bw_recurrent_weights, bw_bias, bw_hidden_state, aux_input, fw_aux_weights, bw_aux_weights, activation, time_major, merge_outputs}, {fw_output, bw_output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, fw_weights, fw_recurrent_weights, fw_bias, fw_hidden_state, bw_weights, bw_recurrent_weights, bw_bias, bw_hidden_state, aux_input, fw_aux_weights, bw_aux_weights},
    {fw_output, bw_output});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16(Model *model) {
  OperandType type14(Type::TENSOR_FLOAT16, {0});
  OperandType type15(Type::TENSOR_FLOAT16, {16});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 16});
  OperandType type17(Type::TENSOR_FLOAT16, {2, 16, 16});
  OperandType type18(Type::TENSOR_FLOAT16, {16, 16});
  OperandType type19(Type::TENSOR_FLOAT16, {16, 8});
  OperandType type20(Type::TENSOR_FLOAT16, {2, 16, 8});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto input = model->addOperand(&type20);
  auto fw_weights = model->addOperand(&type19);
  auto fw_recurrent_weights = model->addOperand(&type18);
  auto fw_bias = model->addOperand(&type15);
  auto fw_hidden_state = model->addOperand(&type16);
  auto bw_weights = model->addOperand(&type19);
  auto bw_recurrent_weights = model->addOperand(&type18);
  auto bw_bias = model->addOperand(&type15);
  auto bw_hidden_state = model->addOperand(&type16);
  auto aux_input = model->addOperand(&type14);
  auto fw_aux_weights = model->addOperand(&type14);
  auto bw_aux_weights = model->addOperand(&type14);
  auto activation = model->addOperand(&type7);
  auto time_major = model->addOperand(&type8);
  auto merge_outputs = model->addOperand(&type8);
  auto fw_output = model->addOperand(&type17);
  auto bw_output = model->addOperand(&type17);
  // Phase 2, operations
  static int32_t activation_init[] = {1};
  model->setOperandValue(activation, activation_init, sizeof(int32_t) * 1);
  static bool8 time_major_init[] = {false};
  model->setOperandValue(time_major, time_major_init, sizeof(bool8) * 1);
  static bool8 merge_outputs_init[] = {false};
  model->setOperandValue(merge_outputs, merge_outputs_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input, fw_weights, fw_recurrent_weights, fw_bias, fw_hidden_state, bw_weights, bw_recurrent_weights, bw_bias, bw_hidden_state, aux_input, fw_aux_weights, bw_aux_weights, activation, time_major, merge_outputs}, {fw_output, bw_output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, fw_weights, fw_recurrent_weights, fw_bias, fw_hidden_state, bw_weights, bw_recurrent_weights, bw_bias, bw_hidden_state, aux_input, fw_aux_weights, bw_aux_weights},
    {fw_output, bw_output});
  assert(model->isValid());
}

inline bool is_ignored_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 16, 8});
  OperandType type1(Type::TENSOR_FLOAT32, {16, 8});
  OperandType type2(Type::TENSOR_FLOAT32, {16, 16});
  OperandType type21(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {16});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 16});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto fw_weights = model->addOperand(&type1);
  auto fw_recurrent_weights = model->addOperand(&type2);
  auto fw_bias = model->addOperand(&type3);
  auto fw_hidden_state = model->addOperand(&type4);
  auto bw_weights = model->addOperand(&type1);
  auto bw_recurrent_weights = model->addOperand(&type2);
  auto bw_bias = model->addOperand(&type3);
  auto bw_hidden_state = model->addOperand(&type4);
  auto aux_input = model->addOperand(&type5);
  auto fw_aux_weights = model->addOperand(&type5);
  auto bw_aux_weights = model->addOperand(&type5);
  auto activation = model->addOperand(&type7);
  auto time_major = model->addOperand(&type8);
  auto merge_outputs = model->addOperand(&type8);
  auto fw_output = model->addOperand(&type21);
  auto bw_output = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t activation_init[] = {1};
  model->setOperandValue(activation, activation_init, sizeof(int32_t) * 1);
  static bool8 time_major_init[] = {false};
  model->setOperandValue(time_major, time_major_init, sizeof(bool8) * 1);
  static bool8 merge_outputs_init[] = {false};
  model->setOperandValue(merge_outputs, merge_outputs_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input, fw_weights, fw_recurrent_weights, fw_bias, fw_hidden_state, bw_weights, bw_recurrent_weights, bw_bias, bw_hidden_state, aux_input, fw_aux_weights, bw_aux_weights, activation, time_major, merge_outputs}, {fw_output, bw_output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, fw_weights, fw_recurrent_weights, fw_bias, fw_hidden_state, bw_weights, bw_recurrent_weights, bw_bias, bw_hidden_state, aux_input, fw_aux_weights, bw_aux_weights},
    {fw_output, bw_output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 16, 8});
  OperandType type1(Type::TENSOR_FLOAT32, {16, 8});
  OperandType type2(Type::TENSOR_FLOAT32, {16, 16});
  OperandType type21(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {16});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 16});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto fw_weights = model->addOperand(&type1);
  auto fw_recurrent_weights = model->addOperand(&type2);
  auto fw_bias = model->addOperand(&type3);
  auto fw_hidden_state = model->addOperand(&type4);
  auto bw_weights = model->addOperand(&type1);
  auto bw_recurrent_weights = model->addOperand(&type2);
  auto bw_bias = model->addOperand(&type3);
  auto bw_hidden_state = model->addOperand(&type4);
  auto aux_input = model->addOperand(&type5);
  auto fw_aux_weights = model->addOperand(&type5);
  auto bw_aux_weights = model->addOperand(&type5);
  auto activation = model->addOperand(&type7);
  auto time_major = model->addOperand(&type8);
  auto merge_outputs = model->addOperand(&type8);
  auto fw_output = model->addOperand(&type21);
  auto bw_output = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t activation_init[] = {1};
  model->setOperandValue(activation, activation_init, sizeof(int32_t) * 1);
  static bool8 time_major_init[] = {false};
  model->setOperandValue(time_major, time_major_init, sizeof(bool8) * 1);
  static bool8 merge_outputs_init[] = {false};
  model->setOperandValue(merge_outputs, merge_outputs_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input, fw_weights, fw_recurrent_weights, fw_bias, fw_hidden_state, bw_weights, bw_recurrent_weights, bw_bias, bw_hidden_state, aux_input, fw_aux_weights, bw_aux_weights, activation, time_major, merge_outputs}, {fw_output, bw_output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, fw_weights, fw_recurrent_weights, fw_bias, fw_hidden_state, bw_weights, bw_recurrent_weights, bw_bias, bw_hidden_state, aux_input, fw_aux_weights, bw_aux_weights},
    {fw_output, bw_output});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16(Model *model) {
  OperandType type14(Type::TENSOR_FLOAT16, {0});
  OperandType type15(Type::TENSOR_FLOAT16, {16});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 16});
  OperandType type18(Type::TENSOR_FLOAT16, {16, 16});
  OperandType type19(Type::TENSOR_FLOAT16, {16, 8});
  OperandType type20(Type::TENSOR_FLOAT16, {2, 16, 8});
  OperandType type22(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto input = model->addOperand(&type20);
  auto fw_weights = model->addOperand(&type19);
  auto fw_recurrent_weights = model->addOperand(&type18);
  auto fw_bias = model->addOperand(&type15);
  auto fw_hidden_state = model->addOperand(&type16);
  auto bw_weights = model->addOperand(&type19);
  auto bw_recurrent_weights = model->addOperand(&type18);
  auto bw_bias = model->addOperand(&type15);
  auto bw_hidden_state = model->addOperand(&type16);
  auto aux_input = model->addOperand(&type14);
  auto fw_aux_weights = model->addOperand(&type14);
  auto bw_aux_weights = model->addOperand(&type14);
  auto activation = model->addOperand(&type7);
  auto time_major = model->addOperand(&type8);
  auto merge_outputs = model->addOperand(&type8);
  auto fw_output = model->addOperand(&type22);
  auto bw_output = model->addOperand(&type22);
  // Phase 2, operations
  static int32_t activation_init[] = {1};
  model->setOperandValue(activation, activation_init, sizeof(int32_t) * 1);
  static bool8 time_major_init[] = {false};
  model->setOperandValue(time_major, time_major_init, sizeof(bool8) * 1);
  static bool8 merge_outputs_init[] = {false};
  model->setOperandValue(merge_outputs, merge_outputs_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input, fw_weights, fw_recurrent_weights, fw_bias, fw_hidden_state, bw_weights, bw_recurrent_weights, bw_bias, bw_hidden_state, aux_input, fw_aux_weights, bw_aux_weights, activation, time_major, merge_outputs}, {fw_output, bw_output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, fw_weights, fw_recurrent_weights, fw_bias, fw_hidden_state, bw_weights, bw_recurrent_weights, bw_bias, bw_hidden_state, aux_input, fw_aux_weights, bw_aux_weights},
    {fw_output, bw_output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_2(Model *model) {
  OperandType type1(Type::TENSOR_FLOAT32, {16, 8});
  OperandType type10(Type::TENSOR_FLOAT32, {16, 2, 16});
  OperandType type2(Type::TENSOR_FLOAT32, {16, 16});
  OperandType type3(Type::TENSOR_FLOAT32, {16});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 16});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::TENSOR_FLOAT32, {16, 2, 8});
  // Phase 1, operands
  auto input1 = model->addOperand(&type9);
  auto fw_weights1 = model->addOperand(&type1);
  auto fw_recurrent_weights1 = model->addOperand(&type2);
  auto fw_bias1 = model->addOperand(&type3);
  auto fw_hidden_state1 = model->addOperand(&type4);
  auto bw_weights1 = model->addOperand(&type1);
  auto bw_recurrent_weights1 = model->addOperand(&type2);
  auto bw_bias1 = model->addOperand(&type3);
  auto bw_hidden_state1 = model->addOperand(&type4);
  auto aux_input1 = model->addOperand(&type5);
  auto fw_aux_weights1 = model->addOperand(&type5);
  auto bw_aux_weights1 = model->addOperand(&type5);
  auto activation1 = model->addOperand(&type7);
  auto time_major1 = model->addOperand(&type8);
  auto merge_outputs1 = model->addOperand(&type8);
  auto fw_output1 = model->addOperand(&type10);
  auto bw_output1 = model->addOperand(&type10);
  // Phase 2, operations
  static int32_t activation1_init[] = {1};
  model->setOperandValue(activation1, activation1_init, sizeof(int32_t) * 1);
  static bool8 time_major1_init[] = {true};
  model->setOperandValue(time_major1, time_major1_init, sizeof(bool8) * 1);
  static bool8 merge_outputs1_init[] = {false};
  model->setOperandValue(merge_outputs1, merge_outputs1_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input1, fw_weights1, fw_recurrent_weights1, fw_bias1, fw_hidden_state1, bw_weights1, bw_recurrent_weights1, bw_bias1, bw_hidden_state1, aux_input1, fw_aux_weights1, bw_aux_weights1, activation1, time_major1, merge_outputs1}, {fw_output1, bw_output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input1, fw_weights1, fw_recurrent_weights1, fw_bias1, fw_hidden_state1, bw_weights1, bw_recurrent_weights1, bw_bias1, bw_hidden_state1, aux_input1, fw_aux_weights1, bw_aux_weights1},
    {fw_output1, bw_output1});
  assert(model->isValid());
}

inline bool is_ignored_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_2(Model *model) {
  OperandType type1(Type::TENSOR_FLOAT32, {16, 8});
  OperandType type10(Type::TENSOR_FLOAT32, {16, 2, 16});
  OperandType type2(Type::TENSOR_FLOAT32, {16, 16});
  OperandType type3(Type::TENSOR_FLOAT32, {16});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 16});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::TENSOR_FLOAT32, {16, 2, 8});
  // Phase 1, operands
  auto input1 = model->addOperand(&type9);
  auto fw_weights1 = model->addOperand(&type1);
  auto fw_recurrent_weights1 = model->addOperand(&type2);
  auto fw_bias1 = model->addOperand(&type3);
  auto fw_hidden_state1 = model->addOperand(&type4);
  auto bw_weights1 = model->addOperand(&type1);
  auto bw_recurrent_weights1 = model->addOperand(&type2);
  auto bw_bias1 = model->addOperand(&type3);
  auto bw_hidden_state1 = model->addOperand(&type4);
  auto aux_input1 = model->addOperand(&type5);
  auto fw_aux_weights1 = model->addOperand(&type5);
  auto bw_aux_weights1 = model->addOperand(&type5);
  auto activation1 = model->addOperand(&type7);
  auto time_major1 = model->addOperand(&type8);
  auto merge_outputs1 = model->addOperand(&type8);
  auto fw_output1 = model->addOperand(&type10);
  auto bw_output1 = model->addOperand(&type10);
  // Phase 2, operations
  static int32_t activation1_init[] = {1};
  model->setOperandValue(activation1, activation1_init, sizeof(int32_t) * 1);
  static bool8 time_major1_init[] = {true};
  model->setOperandValue(time_major1, time_major1_init, sizeof(bool8) * 1);
  static bool8 merge_outputs1_init[] = {false};
  model->setOperandValue(merge_outputs1, merge_outputs1_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input1, fw_weights1, fw_recurrent_weights1, fw_bias1, fw_hidden_state1, bw_weights1, bw_recurrent_weights1, bw_bias1, bw_hidden_state1, aux_input1, fw_aux_weights1, bw_aux_weights1, activation1, time_major1, merge_outputs1}, {fw_output1, bw_output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input1, fw_weights1, fw_recurrent_weights1, fw_bias1, fw_hidden_state1, bw_weights1, bw_recurrent_weights1, bw_bias1, bw_hidden_state1, aux_input1, fw_aux_weights1, bw_aux_weights1},
    {fw_output1, bw_output1});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_2(Model *model) {
  OperandType type14(Type::TENSOR_FLOAT16, {0});
  OperandType type15(Type::TENSOR_FLOAT16, {16});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 16});
  OperandType type18(Type::TENSOR_FLOAT16, {16, 16});
  OperandType type19(Type::TENSOR_FLOAT16, {16, 8});
  OperandType type23(Type::TENSOR_FLOAT16, {16, 2, 16});
  OperandType type24(Type::TENSOR_FLOAT16, {16, 2, 8});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto input1 = model->addOperand(&type24);
  auto fw_weights1 = model->addOperand(&type19);
  auto fw_recurrent_weights1 = model->addOperand(&type18);
  auto fw_bias1 = model->addOperand(&type15);
  auto fw_hidden_state1 = model->addOperand(&type16);
  auto bw_weights1 = model->addOperand(&type19);
  auto bw_recurrent_weights1 = model->addOperand(&type18);
  auto bw_bias1 = model->addOperand(&type15);
  auto bw_hidden_state1 = model->addOperand(&type16);
  auto aux_input1 = model->addOperand(&type14);
  auto fw_aux_weights1 = model->addOperand(&type14);
  auto bw_aux_weights1 = model->addOperand(&type14);
  auto activation1 = model->addOperand(&type7);
  auto time_major1 = model->addOperand(&type8);
  auto merge_outputs1 = model->addOperand(&type8);
  auto fw_output1 = model->addOperand(&type23);
  auto bw_output1 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t activation1_init[] = {1};
  model->setOperandValue(activation1, activation1_init, sizeof(int32_t) * 1);
  static bool8 time_major1_init[] = {true};
  model->setOperandValue(time_major1, time_major1_init, sizeof(bool8) * 1);
  static bool8 merge_outputs1_init[] = {false};
  model->setOperandValue(merge_outputs1, merge_outputs1_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input1, fw_weights1, fw_recurrent_weights1, fw_bias1, fw_hidden_state1, bw_weights1, bw_recurrent_weights1, bw_bias1, bw_hidden_state1, aux_input1, fw_aux_weights1, bw_aux_weights1, activation1, time_major1, merge_outputs1}, {fw_output1, bw_output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input1, fw_weights1, fw_recurrent_weights1, fw_bias1, fw_hidden_state1, bw_weights1, bw_recurrent_weights1, bw_bias1, bw_hidden_state1, aux_input1, fw_aux_weights1, bw_aux_weights1},
    {fw_output1, bw_output1});
  assert(model->isValid());
}

inline bool is_ignored_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_2(Model *model) {
  OperandType type1(Type::TENSOR_FLOAT32, {16, 8});
  OperandType type2(Type::TENSOR_FLOAT32, {16, 16});
  OperandType type21(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {16});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 16});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::TENSOR_FLOAT32, {16, 2, 8});
  // Phase 1, operands
  auto input1 = model->addOperand(&type9);
  auto fw_weights1 = model->addOperand(&type1);
  auto fw_recurrent_weights1 = model->addOperand(&type2);
  auto fw_bias1 = model->addOperand(&type3);
  auto fw_hidden_state1 = model->addOperand(&type4);
  auto bw_weights1 = model->addOperand(&type1);
  auto bw_recurrent_weights1 = model->addOperand(&type2);
  auto bw_bias1 = model->addOperand(&type3);
  auto bw_hidden_state1 = model->addOperand(&type4);
  auto aux_input1 = model->addOperand(&type5);
  auto fw_aux_weights1 = model->addOperand(&type5);
  auto bw_aux_weights1 = model->addOperand(&type5);
  auto activation1 = model->addOperand(&type7);
  auto time_major1 = model->addOperand(&type8);
  auto merge_outputs1 = model->addOperand(&type8);
  auto fw_output1 = model->addOperand(&type21);
  auto bw_output1 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t activation1_init[] = {1};
  model->setOperandValue(activation1, activation1_init, sizeof(int32_t) * 1);
  static bool8 time_major1_init[] = {true};
  model->setOperandValue(time_major1, time_major1_init, sizeof(bool8) * 1);
  static bool8 merge_outputs1_init[] = {false};
  model->setOperandValue(merge_outputs1, merge_outputs1_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input1, fw_weights1, fw_recurrent_weights1, fw_bias1, fw_hidden_state1, bw_weights1, bw_recurrent_weights1, bw_bias1, bw_hidden_state1, aux_input1, fw_aux_weights1, bw_aux_weights1, activation1, time_major1, merge_outputs1}, {fw_output1, bw_output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input1, fw_weights1, fw_recurrent_weights1, fw_bias1, fw_hidden_state1, bw_weights1, bw_recurrent_weights1, bw_bias1, bw_hidden_state1, aux_input1, fw_aux_weights1, bw_aux_weights1},
    {fw_output1, bw_output1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_2(Model *model) {
  OperandType type1(Type::TENSOR_FLOAT32, {16, 8});
  OperandType type2(Type::TENSOR_FLOAT32, {16, 16});
  OperandType type21(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {16});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 16});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::TENSOR_FLOAT32, {16, 2, 8});
  // Phase 1, operands
  auto input1 = model->addOperand(&type9);
  auto fw_weights1 = model->addOperand(&type1);
  auto fw_recurrent_weights1 = model->addOperand(&type2);
  auto fw_bias1 = model->addOperand(&type3);
  auto fw_hidden_state1 = model->addOperand(&type4);
  auto bw_weights1 = model->addOperand(&type1);
  auto bw_recurrent_weights1 = model->addOperand(&type2);
  auto bw_bias1 = model->addOperand(&type3);
  auto bw_hidden_state1 = model->addOperand(&type4);
  auto aux_input1 = model->addOperand(&type5);
  auto fw_aux_weights1 = model->addOperand(&type5);
  auto bw_aux_weights1 = model->addOperand(&type5);
  auto activation1 = model->addOperand(&type7);
  auto time_major1 = model->addOperand(&type8);
  auto merge_outputs1 = model->addOperand(&type8);
  auto fw_output1 = model->addOperand(&type21);
  auto bw_output1 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t activation1_init[] = {1};
  model->setOperandValue(activation1, activation1_init, sizeof(int32_t) * 1);
  static bool8 time_major1_init[] = {true};
  model->setOperandValue(time_major1, time_major1_init, sizeof(bool8) * 1);
  static bool8 merge_outputs1_init[] = {false};
  model->setOperandValue(merge_outputs1, merge_outputs1_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input1, fw_weights1, fw_recurrent_weights1, fw_bias1, fw_hidden_state1, bw_weights1, bw_recurrent_weights1, bw_bias1, bw_hidden_state1, aux_input1, fw_aux_weights1, bw_aux_weights1, activation1, time_major1, merge_outputs1}, {fw_output1, bw_output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input1, fw_weights1, fw_recurrent_weights1, fw_bias1, fw_hidden_state1, bw_weights1, bw_recurrent_weights1, bw_bias1, bw_hidden_state1, aux_input1, fw_aux_weights1, bw_aux_weights1},
    {fw_output1, bw_output1});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_2(Model *model) {
  OperandType type14(Type::TENSOR_FLOAT16, {0});
  OperandType type15(Type::TENSOR_FLOAT16, {16});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 16});
  OperandType type18(Type::TENSOR_FLOAT16, {16, 16});
  OperandType type19(Type::TENSOR_FLOAT16, {16, 8});
  OperandType type22(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type24(Type::TENSOR_FLOAT16, {16, 2, 8});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto input1 = model->addOperand(&type24);
  auto fw_weights1 = model->addOperand(&type19);
  auto fw_recurrent_weights1 = model->addOperand(&type18);
  auto fw_bias1 = model->addOperand(&type15);
  auto fw_hidden_state1 = model->addOperand(&type16);
  auto bw_weights1 = model->addOperand(&type19);
  auto bw_recurrent_weights1 = model->addOperand(&type18);
  auto bw_bias1 = model->addOperand(&type15);
  auto bw_hidden_state1 = model->addOperand(&type16);
  auto aux_input1 = model->addOperand(&type14);
  auto fw_aux_weights1 = model->addOperand(&type14);
  auto bw_aux_weights1 = model->addOperand(&type14);
  auto activation1 = model->addOperand(&type7);
  auto time_major1 = model->addOperand(&type8);
  auto merge_outputs1 = model->addOperand(&type8);
  auto fw_output1 = model->addOperand(&type22);
  auto bw_output1 = model->addOperand(&type22);
  // Phase 2, operations
  static int32_t activation1_init[] = {1};
  model->setOperandValue(activation1, activation1_init, sizeof(int32_t) * 1);
  static bool8 time_major1_init[] = {true};
  model->setOperandValue(time_major1, time_major1_init, sizeof(bool8) * 1);
  static bool8 merge_outputs1_init[] = {false};
  model->setOperandValue(merge_outputs1, merge_outputs1_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input1, fw_weights1, fw_recurrent_weights1, fw_bias1, fw_hidden_state1, bw_weights1, bw_recurrent_weights1, bw_bias1, bw_hidden_state1, aux_input1, fw_aux_weights1, bw_aux_weights1, activation1, time_major1, merge_outputs1}, {fw_output1, bw_output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input1, fw_weights1, fw_recurrent_weights1, fw_bias1, fw_hidden_state1, bw_weights1, bw_recurrent_weights1, bw_bias1, bw_hidden_state1, aux_input1, fw_aux_weights1, bw_aux_weights1},
    {fw_output1, bw_output1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3(Model *model) {
  OperandType type1(Type::TENSOR_FLOAT32, {16, 8});
  OperandType type11(Type::TENSOR_FLOAT32, {16, 2, 32});
  OperandType type2(Type::TENSOR_FLOAT32, {16, 16});
  OperandType type3(Type::TENSOR_FLOAT32, {16});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 16});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::TENSOR_FLOAT32, {16, 2, 8});
  // Phase 1, operands
  auto input2 = model->addOperand(&type9);
  auto fw_weights2 = model->addOperand(&type1);
  auto fw_recurrent_weights2 = model->addOperand(&type2);
  auto fw_bias2 = model->addOperand(&type3);
  auto fw_hidden_state2 = model->addOperand(&type4);
  auto bw_weights2 = model->addOperand(&type1);
  auto bw_recurrent_weights2 = model->addOperand(&type2);
  auto bw_bias2 = model->addOperand(&type3);
  auto bw_hidden_state2 = model->addOperand(&type4);
  auto aux_input2 = model->addOperand(&type5);
  auto fw_aux_weights2 = model->addOperand(&type5);
  auto bw_aux_weights2 = model->addOperand(&type5);
  auto activation2 = model->addOperand(&type7);
  auto time_major2 = model->addOperand(&type8);
  auto merge_outputs2 = model->addOperand(&type8);
  auto fw_output2 = model->addOperand(&type11);
  // Phase 2, operations
  static int32_t activation2_init[] = {1};
  model->setOperandValue(activation2, activation2_init, sizeof(int32_t) * 1);
  static bool8 time_major2_init[] = {true};
  model->setOperandValue(time_major2, time_major2_init, sizeof(bool8) * 1);
  static bool8 merge_outputs2_init[] = {true};
  model->setOperandValue(merge_outputs2, merge_outputs2_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input2, fw_weights2, fw_recurrent_weights2, fw_bias2, fw_hidden_state2, bw_weights2, bw_recurrent_weights2, bw_bias2, bw_hidden_state2, aux_input2, fw_aux_weights2, bw_aux_weights2, activation2, time_major2, merge_outputs2}, {fw_output2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input2, fw_weights2, fw_recurrent_weights2, fw_bias2, fw_hidden_state2, bw_weights2, bw_recurrent_weights2, bw_bias2, bw_hidden_state2, aux_input2, fw_aux_weights2, bw_aux_weights2},
    {fw_output2});
  assert(model->isValid());
}

inline bool is_ignored_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_3(Model *model) {
  OperandType type1(Type::TENSOR_FLOAT32, {16, 8});
  OperandType type11(Type::TENSOR_FLOAT32, {16, 2, 32});
  OperandType type2(Type::TENSOR_FLOAT32, {16, 16});
  OperandType type3(Type::TENSOR_FLOAT32, {16});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 16});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::TENSOR_FLOAT32, {16, 2, 8});
  // Phase 1, operands
  auto input2 = model->addOperand(&type9);
  auto fw_weights2 = model->addOperand(&type1);
  auto fw_recurrent_weights2 = model->addOperand(&type2);
  auto fw_bias2 = model->addOperand(&type3);
  auto fw_hidden_state2 = model->addOperand(&type4);
  auto bw_weights2 = model->addOperand(&type1);
  auto bw_recurrent_weights2 = model->addOperand(&type2);
  auto bw_bias2 = model->addOperand(&type3);
  auto bw_hidden_state2 = model->addOperand(&type4);
  auto aux_input2 = model->addOperand(&type5);
  auto fw_aux_weights2 = model->addOperand(&type5);
  auto bw_aux_weights2 = model->addOperand(&type5);
  auto activation2 = model->addOperand(&type7);
  auto time_major2 = model->addOperand(&type8);
  auto merge_outputs2 = model->addOperand(&type8);
  auto fw_output2 = model->addOperand(&type11);
  // Phase 2, operations
  static int32_t activation2_init[] = {1};
  model->setOperandValue(activation2, activation2_init, sizeof(int32_t) * 1);
  static bool8 time_major2_init[] = {true};
  model->setOperandValue(time_major2, time_major2_init, sizeof(bool8) * 1);
  static bool8 merge_outputs2_init[] = {true};
  model->setOperandValue(merge_outputs2, merge_outputs2_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input2, fw_weights2, fw_recurrent_weights2, fw_bias2, fw_hidden_state2, bw_weights2, bw_recurrent_weights2, bw_bias2, bw_hidden_state2, aux_input2, fw_aux_weights2, bw_aux_weights2, activation2, time_major2, merge_outputs2}, {fw_output2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input2, fw_weights2, fw_recurrent_weights2, fw_bias2, fw_hidden_state2, bw_weights2, bw_recurrent_weights2, bw_bias2, bw_hidden_state2, aux_input2, fw_aux_weights2, bw_aux_weights2},
    {fw_output2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_3(Model *model) {
  OperandType type14(Type::TENSOR_FLOAT16, {0});
  OperandType type15(Type::TENSOR_FLOAT16, {16});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 16});
  OperandType type18(Type::TENSOR_FLOAT16, {16, 16});
  OperandType type19(Type::TENSOR_FLOAT16, {16, 8});
  OperandType type24(Type::TENSOR_FLOAT16, {16, 2, 8});
  OperandType type25(Type::TENSOR_FLOAT16, {16, 2, 32});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto input2 = model->addOperand(&type24);
  auto fw_weights2 = model->addOperand(&type19);
  auto fw_recurrent_weights2 = model->addOperand(&type18);
  auto fw_bias2 = model->addOperand(&type15);
  auto fw_hidden_state2 = model->addOperand(&type16);
  auto bw_weights2 = model->addOperand(&type19);
  auto bw_recurrent_weights2 = model->addOperand(&type18);
  auto bw_bias2 = model->addOperand(&type15);
  auto bw_hidden_state2 = model->addOperand(&type16);
  auto aux_input2 = model->addOperand(&type14);
  auto fw_aux_weights2 = model->addOperand(&type14);
  auto bw_aux_weights2 = model->addOperand(&type14);
  auto activation2 = model->addOperand(&type7);
  auto time_major2 = model->addOperand(&type8);
  auto merge_outputs2 = model->addOperand(&type8);
  auto fw_output2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t activation2_init[] = {1};
  model->setOperandValue(activation2, activation2_init, sizeof(int32_t) * 1);
  static bool8 time_major2_init[] = {true};
  model->setOperandValue(time_major2, time_major2_init, sizeof(bool8) * 1);
  static bool8 merge_outputs2_init[] = {true};
  model->setOperandValue(merge_outputs2, merge_outputs2_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input2, fw_weights2, fw_recurrent_weights2, fw_bias2, fw_hidden_state2, bw_weights2, bw_recurrent_weights2, bw_bias2, bw_hidden_state2, aux_input2, fw_aux_weights2, bw_aux_weights2, activation2, time_major2, merge_outputs2}, {fw_output2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input2, fw_weights2, fw_recurrent_weights2, fw_bias2, fw_hidden_state2, bw_weights2, bw_recurrent_weights2, bw_bias2, bw_hidden_state2, aux_input2, fw_aux_weights2, bw_aux_weights2},
    {fw_output2});
  assert(model->isValid());
}

inline bool is_ignored_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_3(Model *model) {
  OperandType type1(Type::TENSOR_FLOAT32, {16, 8});
  OperandType type2(Type::TENSOR_FLOAT32, {16, 16});
  OperandType type21(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {16});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 16});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::TENSOR_FLOAT32, {16, 2, 8});
  // Phase 1, operands
  auto input2 = model->addOperand(&type9);
  auto fw_weights2 = model->addOperand(&type1);
  auto fw_recurrent_weights2 = model->addOperand(&type2);
  auto fw_bias2 = model->addOperand(&type3);
  auto fw_hidden_state2 = model->addOperand(&type4);
  auto bw_weights2 = model->addOperand(&type1);
  auto bw_recurrent_weights2 = model->addOperand(&type2);
  auto bw_bias2 = model->addOperand(&type3);
  auto bw_hidden_state2 = model->addOperand(&type4);
  auto aux_input2 = model->addOperand(&type5);
  auto fw_aux_weights2 = model->addOperand(&type5);
  auto bw_aux_weights2 = model->addOperand(&type5);
  auto activation2 = model->addOperand(&type7);
  auto time_major2 = model->addOperand(&type8);
  auto merge_outputs2 = model->addOperand(&type8);
  auto fw_output2 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t activation2_init[] = {1};
  model->setOperandValue(activation2, activation2_init, sizeof(int32_t) * 1);
  static bool8 time_major2_init[] = {true};
  model->setOperandValue(time_major2, time_major2_init, sizeof(bool8) * 1);
  static bool8 merge_outputs2_init[] = {true};
  model->setOperandValue(merge_outputs2, merge_outputs2_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input2, fw_weights2, fw_recurrent_weights2, fw_bias2, fw_hidden_state2, bw_weights2, bw_recurrent_weights2, bw_bias2, bw_hidden_state2, aux_input2, fw_aux_weights2, bw_aux_weights2, activation2, time_major2, merge_outputs2}, {fw_output2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input2, fw_weights2, fw_recurrent_weights2, fw_bias2, fw_hidden_state2, bw_weights2, bw_recurrent_weights2, bw_bias2, bw_hidden_state2, aux_input2, fw_aux_weights2, bw_aux_weights2},
    {fw_output2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_3(Model *model) {
  OperandType type1(Type::TENSOR_FLOAT32, {16, 8});
  OperandType type2(Type::TENSOR_FLOAT32, {16, 16});
  OperandType type21(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {16});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 16});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::TENSOR_FLOAT32, {16, 2, 8});
  // Phase 1, operands
  auto input2 = model->addOperand(&type9);
  auto fw_weights2 = model->addOperand(&type1);
  auto fw_recurrent_weights2 = model->addOperand(&type2);
  auto fw_bias2 = model->addOperand(&type3);
  auto fw_hidden_state2 = model->addOperand(&type4);
  auto bw_weights2 = model->addOperand(&type1);
  auto bw_recurrent_weights2 = model->addOperand(&type2);
  auto bw_bias2 = model->addOperand(&type3);
  auto bw_hidden_state2 = model->addOperand(&type4);
  auto aux_input2 = model->addOperand(&type5);
  auto fw_aux_weights2 = model->addOperand(&type5);
  auto bw_aux_weights2 = model->addOperand(&type5);
  auto activation2 = model->addOperand(&type7);
  auto time_major2 = model->addOperand(&type8);
  auto merge_outputs2 = model->addOperand(&type8);
  auto fw_output2 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t activation2_init[] = {1};
  model->setOperandValue(activation2, activation2_init, sizeof(int32_t) * 1);
  static bool8 time_major2_init[] = {true};
  model->setOperandValue(time_major2, time_major2_init, sizeof(bool8) * 1);
  static bool8 merge_outputs2_init[] = {true};
  model->setOperandValue(merge_outputs2, merge_outputs2_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input2, fw_weights2, fw_recurrent_weights2, fw_bias2, fw_hidden_state2, bw_weights2, bw_recurrent_weights2, bw_bias2, bw_hidden_state2, aux_input2, fw_aux_weights2, bw_aux_weights2, activation2, time_major2, merge_outputs2}, {fw_output2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input2, fw_weights2, fw_recurrent_weights2, fw_bias2, fw_hidden_state2, bw_weights2, bw_recurrent_weights2, bw_bias2, bw_hidden_state2, aux_input2, fw_aux_weights2, bw_aux_weights2},
    {fw_output2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_3(Model *model) {
  OperandType type14(Type::TENSOR_FLOAT16, {0});
  OperandType type15(Type::TENSOR_FLOAT16, {16});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 16});
  OperandType type18(Type::TENSOR_FLOAT16, {16, 16});
  OperandType type19(Type::TENSOR_FLOAT16, {16, 8});
  OperandType type22(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type24(Type::TENSOR_FLOAT16, {16, 2, 8});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto input2 = model->addOperand(&type24);
  auto fw_weights2 = model->addOperand(&type19);
  auto fw_recurrent_weights2 = model->addOperand(&type18);
  auto fw_bias2 = model->addOperand(&type15);
  auto fw_hidden_state2 = model->addOperand(&type16);
  auto bw_weights2 = model->addOperand(&type19);
  auto bw_recurrent_weights2 = model->addOperand(&type18);
  auto bw_bias2 = model->addOperand(&type15);
  auto bw_hidden_state2 = model->addOperand(&type16);
  auto aux_input2 = model->addOperand(&type14);
  auto fw_aux_weights2 = model->addOperand(&type14);
  auto bw_aux_weights2 = model->addOperand(&type14);
  auto activation2 = model->addOperand(&type7);
  auto time_major2 = model->addOperand(&type8);
  auto merge_outputs2 = model->addOperand(&type8);
  auto fw_output2 = model->addOperand(&type22);
  // Phase 2, operations
  static int32_t activation2_init[] = {1};
  model->setOperandValue(activation2, activation2_init, sizeof(int32_t) * 1);
  static bool8 time_major2_init[] = {true};
  model->setOperandValue(time_major2, time_major2_init, sizeof(bool8) * 1);
  static bool8 merge_outputs2_init[] = {true};
  model->setOperandValue(merge_outputs2, merge_outputs2_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input2, fw_weights2, fw_recurrent_weights2, fw_bias2, fw_hidden_state2, bw_weights2, bw_recurrent_weights2, bw_bias2, bw_hidden_state2, aux_input2, fw_aux_weights2, bw_aux_weights2, activation2, time_major2, merge_outputs2}, {fw_output2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input2, fw_weights2, fw_recurrent_weights2, fw_bias2, fw_hidden_state2, bw_weights2, bw_recurrent_weights2, bw_bias2, bw_hidden_state2, aux_input2, fw_aux_weights2, bw_aux_weights2},
    {fw_output2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_4(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 16, 8});
  OperandType type1(Type::TENSOR_FLOAT32, {16, 8});
  OperandType type2(Type::TENSOR_FLOAT32, {16, 16});
  OperandType type3(Type::TENSOR_FLOAT32, {16});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 16});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 16, 16});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto input3 = model->addOperand(&type0);
  auto fw_weights3 = model->addOperand(&type1);
  auto fw_recurrent_weights3 = model->addOperand(&type2);
  auto fw_bias3 = model->addOperand(&type3);
  auto fw_hidden_state3 = model->addOperand(&type4);
  auto bw_weights3 = model->addOperand(&type1);
  auto bw_recurrent_weights3 = model->addOperand(&type2);
  auto bw_bias3 = model->addOperand(&type3);
  auto bw_hidden_state3 = model->addOperand(&type4);
  auto aux_input3 = model->addOperand(&type5);
  auto fw_aux_weights3 = model->addOperand(&type5);
  auto bw_aux_weights3 = model->addOperand(&type5);
  auto activation3 = model->addOperand(&type7);
  auto time_major3 = model->addOperand(&type8);
  auto merge_outputs3 = model->addOperand(&type8);
  auto fw_output3 = model->addOperand(&type6);
  auto bw_output2 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t activation3_init[] = {1};
  model->setOperandValue(activation3, activation3_init, sizeof(int32_t) * 1);
  static bool8 time_major3_init[] = {false};
  model->setOperandValue(time_major3, time_major3_init, sizeof(bool8) * 1);
  static bool8 merge_outputs3_init[] = {false};
  model->setOperandValue(merge_outputs3, merge_outputs3_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input3, fw_weights3, fw_recurrent_weights3, fw_bias3, fw_hidden_state3, bw_weights3, bw_recurrent_weights3, bw_bias3, bw_hidden_state3, aux_input3, fw_aux_weights3, bw_aux_weights3, activation3, time_major3, merge_outputs3}, {fw_output3, bw_output2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input3, fw_weights3, fw_recurrent_weights3, fw_bias3, fw_hidden_state3, bw_weights3, bw_recurrent_weights3, bw_bias3, bw_hidden_state3, aux_input3, fw_aux_weights3, bw_aux_weights3},
    {fw_output3, bw_output2});
  assert(model->isValid());
}

inline bool is_ignored_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_4(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 16, 8});
  OperandType type1(Type::TENSOR_FLOAT32, {16, 8});
  OperandType type2(Type::TENSOR_FLOAT32, {16, 16});
  OperandType type3(Type::TENSOR_FLOAT32, {16});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 16});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 16, 16});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto input3 = model->addOperand(&type0);
  auto fw_weights3 = model->addOperand(&type1);
  auto fw_recurrent_weights3 = model->addOperand(&type2);
  auto fw_bias3 = model->addOperand(&type3);
  auto fw_hidden_state3 = model->addOperand(&type4);
  auto bw_weights3 = model->addOperand(&type1);
  auto bw_recurrent_weights3 = model->addOperand(&type2);
  auto bw_bias3 = model->addOperand(&type3);
  auto bw_hidden_state3 = model->addOperand(&type4);
  auto aux_input3 = model->addOperand(&type5);
  auto fw_aux_weights3 = model->addOperand(&type5);
  auto bw_aux_weights3 = model->addOperand(&type5);
  auto activation3 = model->addOperand(&type7);
  auto time_major3 = model->addOperand(&type8);
  auto merge_outputs3 = model->addOperand(&type8);
  auto fw_output3 = model->addOperand(&type6);
  auto bw_output2 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t activation3_init[] = {1};
  model->setOperandValue(activation3, activation3_init, sizeof(int32_t) * 1);
  static bool8 time_major3_init[] = {false};
  model->setOperandValue(time_major3, time_major3_init, sizeof(bool8) * 1);
  static bool8 merge_outputs3_init[] = {false};
  model->setOperandValue(merge_outputs3, merge_outputs3_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input3, fw_weights3, fw_recurrent_weights3, fw_bias3, fw_hidden_state3, bw_weights3, bw_recurrent_weights3, bw_bias3, bw_hidden_state3, aux_input3, fw_aux_weights3, bw_aux_weights3, activation3, time_major3, merge_outputs3}, {fw_output3, bw_output2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input3, fw_weights3, fw_recurrent_weights3, fw_bias3, fw_hidden_state3, bw_weights3, bw_recurrent_weights3, bw_bias3, bw_hidden_state3, aux_input3, fw_aux_weights3, bw_aux_weights3},
    {fw_output3, bw_output2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_4(Model *model) {
  OperandType type14(Type::TENSOR_FLOAT16, {0});
  OperandType type15(Type::TENSOR_FLOAT16, {16});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 16});
  OperandType type17(Type::TENSOR_FLOAT16, {2, 16, 16});
  OperandType type18(Type::TENSOR_FLOAT16, {16, 16});
  OperandType type19(Type::TENSOR_FLOAT16, {16, 8});
  OperandType type20(Type::TENSOR_FLOAT16, {2, 16, 8});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto input3 = model->addOperand(&type20);
  auto fw_weights3 = model->addOperand(&type19);
  auto fw_recurrent_weights3 = model->addOperand(&type18);
  auto fw_bias3 = model->addOperand(&type15);
  auto fw_hidden_state3 = model->addOperand(&type16);
  auto bw_weights3 = model->addOperand(&type19);
  auto bw_recurrent_weights3 = model->addOperand(&type18);
  auto bw_bias3 = model->addOperand(&type15);
  auto bw_hidden_state3 = model->addOperand(&type16);
  auto aux_input3 = model->addOperand(&type14);
  auto fw_aux_weights3 = model->addOperand(&type14);
  auto bw_aux_weights3 = model->addOperand(&type14);
  auto activation3 = model->addOperand(&type7);
  auto time_major3 = model->addOperand(&type8);
  auto merge_outputs3 = model->addOperand(&type8);
  auto fw_output3 = model->addOperand(&type17);
  auto bw_output2 = model->addOperand(&type17);
  // Phase 2, operations
  static int32_t activation3_init[] = {1};
  model->setOperandValue(activation3, activation3_init, sizeof(int32_t) * 1);
  static bool8 time_major3_init[] = {false};
  model->setOperandValue(time_major3, time_major3_init, sizeof(bool8) * 1);
  static bool8 merge_outputs3_init[] = {false};
  model->setOperandValue(merge_outputs3, merge_outputs3_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input3, fw_weights3, fw_recurrent_weights3, fw_bias3, fw_hidden_state3, bw_weights3, bw_recurrent_weights3, bw_bias3, bw_hidden_state3, aux_input3, fw_aux_weights3, bw_aux_weights3, activation3, time_major3, merge_outputs3}, {fw_output3, bw_output2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input3, fw_weights3, fw_recurrent_weights3, fw_bias3, fw_hidden_state3, bw_weights3, bw_recurrent_weights3, bw_bias3, bw_hidden_state3, aux_input3, fw_aux_weights3, bw_aux_weights3},
    {fw_output3, bw_output2});
  assert(model->isValid());
}

inline bool is_ignored_float16_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_4(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 16, 8});
  OperandType type1(Type::TENSOR_FLOAT32, {16, 8});
  OperandType type2(Type::TENSOR_FLOAT32, {16, 16});
  OperandType type21(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {16});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 16});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto input3 = model->addOperand(&type0);
  auto fw_weights3 = model->addOperand(&type1);
  auto fw_recurrent_weights3 = model->addOperand(&type2);
  auto fw_bias3 = model->addOperand(&type3);
  auto fw_hidden_state3 = model->addOperand(&type4);
  auto bw_weights3 = model->addOperand(&type1);
  auto bw_recurrent_weights3 = model->addOperand(&type2);
  auto bw_bias3 = model->addOperand(&type3);
  auto bw_hidden_state3 = model->addOperand(&type4);
  auto aux_input3 = model->addOperand(&type5);
  auto fw_aux_weights3 = model->addOperand(&type5);
  auto bw_aux_weights3 = model->addOperand(&type5);
  auto activation3 = model->addOperand(&type7);
  auto time_major3 = model->addOperand(&type8);
  auto merge_outputs3 = model->addOperand(&type8);
  auto fw_output3 = model->addOperand(&type21);
  auto bw_output2 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t activation3_init[] = {1};
  model->setOperandValue(activation3, activation3_init, sizeof(int32_t) * 1);
  static bool8 time_major3_init[] = {false};
  model->setOperandValue(time_major3, time_major3_init, sizeof(bool8) * 1);
  static bool8 merge_outputs3_init[] = {false};
  model->setOperandValue(merge_outputs3, merge_outputs3_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input3, fw_weights3, fw_recurrent_weights3, fw_bias3, fw_hidden_state3, bw_weights3, bw_recurrent_weights3, bw_bias3, bw_hidden_state3, aux_input3, fw_aux_weights3, bw_aux_weights3, activation3, time_major3, merge_outputs3}, {fw_output3, bw_output2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input3, fw_weights3, fw_recurrent_weights3, fw_bias3, fw_hidden_state3, bw_weights3, bw_recurrent_weights3, bw_bias3, bw_hidden_state3, aux_input3, fw_aux_weights3, bw_aux_weights3},
    {fw_output3, bw_output2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_4(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 16, 8});
  OperandType type1(Type::TENSOR_FLOAT32, {16, 8});
  OperandType type2(Type::TENSOR_FLOAT32, {16, 16});
  OperandType type21(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {16});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 16});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto input3 = model->addOperand(&type0);
  auto fw_weights3 = model->addOperand(&type1);
  auto fw_recurrent_weights3 = model->addOperand(&type2);
  auto fw_bias3 = model->addOperand(&type3);
  auto fw_hidden_state3 = model->addOperand(&type4);
  auto bw_weights3 = model->addOperand(&type1);
  auto bw_recurrent_weights3 = model->addOperand(&type2);
  auto bw_bias3 = model->addOperand(&type3);
  auto bw_hidden_state3 = model->addOperand(&type4);
  auto aux_input3 = model->addOperand(&type5);
  auto fw_aux_weights3 = model->addOperand(&type5);
  auto bw_aux_weights3 = model->addOperand(&type5);
  auto activation3 = model->addOperand(&type7);
  auto time_major3 = model->addOperand(&type8);
  auto merge_outputs3 = model->addOperand(&type8);
  auto fw_output3 = model->addOperand(&type21);
  auto bw_output2 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t activation3_init[] = {1};
  model->setOperandValue(activation3, activation3_init, sizeof(int32_t) * 1);
  static bool8 time_major3_init[] = {false};
  model->setOperandValue(time_major3, time_major3_init, sizeof(bool8) * 1);
  static bool8 merge_outputs3_init[] = {false};
  model->setOperandValue(merge_outputs3, merge_outputs3_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input3, fw_weights3, fw_recurrent_weights3, fw_bias3, fw_hidden_state3, bw_weights3, bw_recurrent_weights3, bw_bias3, bw_hidden_state3, aux_input3, fw_aux_weights3, bw_aux_weights3, activation3, time_major3, merge_outputs3}, {fw_output3, bw_output2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input3, fw_weights3, fw_recurrent_weights3, fw_bias3, fw_hidden_state3, bw_weights3, bw_recurrent_weights3, bw_bias3, bw_hidden_state3, aux_input3, fw_aux_weights3, bw_aux_weights3},
    {fw_output3, bw_output2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_4(Model *model) {
  OperandType type14(Type::TENSOR_FLOAT16, {0});
  OperandType type15(Type::TENSOR_FLOAT16, {16});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 16});
  OperandType type18(Type::TENSOR_FLOAT16, {16, 16});
  OperandType type19(Type::TENSOR_FLOAT16, {16, 8});
  OperandType type20(Type::TENSOR_FLOAT16, {2, 16, 8});
  OperandType type22(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto input3 = model->addOperand(&type20);
  auto fw_weights3 = model->addOperand(&type19);
  auto fw_recurrent_weights3 = model->addOperand(&type18);
  auto fw_bias3 = model->addOperand(&type15);
  auto fw_hidden_state3 = model->addOperand(&type16);
  auto bw_weights3 = model->addOperand(&type19);
  auto bw_recurrent_weights3 = model->addOperand(&type18);
  auto bw_bias3 = model->addOperand(&type15);
  auto bw_hidden_state3 = model->addOperand(&type16);
  auto aux_input3 = model->addOperand(&type14);
  auto fw_aux_weights3 = model->addOperand(&type14);
  auto bw_aux_weights3 = model->addOperand(&type14);
  auto activation3 = model->addOperand(&type7);
  auto time_major3 = model->addOperand(&type8);
  auto merge_outputs3 = model->addOperand(&type8);
  auto fw_output3 = model->addOperand(&type22);
  auto bw_output2 = model->addOperand(&type22);
  // Phase 2, operations
  static int32_t activation3_init[] = {1};
  model->setOperandValue(activation3, activation3_init, sizeof(int32_t) * 1);
  static bool8 time_major3_init[] = {false};
  model->setOperandValue(time_major3, time_major3_init, sizeof(bool8) * 1);
  static bool8 merge_outputs3_init[] = {false};
  model->setOperandValue(merge_outputs3, merge_outputs3_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input3, fw_weights3, fw_recurrent_weights3, fw_bias3, fw_hidden_state3, bw_weights3, bw_recurrent_weights3, bw_bias3, bw_hidden_state3, aux_input3, fw_aux_weights3, bw_aux_weights3, activation3, time_major3, merge_outputs3}, {fw_output3, bw_output2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input3, fw_weights3, fw_recurrent_weights3, fw_bias3, fw_hidden_state3, bw_weights3, bw_recurrent_weights3, bw_bias3, bw_hidden_state3, aux_input3, fw_aux_weights3, bw_aux_weights3},
    {fw_output3, bw_output2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_5(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 16, 8});
  OperandType type1(Type::TENSOR_FLOAT32, {16, 8});
  OperandType type2(Type::TENSOR_FLOAT32, {16, 16});
  OperandType type3(Type::TENSOR_FLOAT32, {16});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 16});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 16, 16});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto input4 = model->addOperand(&type0);
  auto fw_weights4 = model->addOperand(&type1);
  auto fw_recurrent_weights4 = model->addOperand(&type2);
  auto fw_bias4 = model->addOperand(&type3);
  auto fw_hidden_state4 = model->addOperand(&type4);
  auto bw_weights4 = model->addOperand(&type1);
  auto bw_recurrent_weights4 = model->addOperand(&type2);
  auto bw_bias4 = model->addOperand(&type3);
  auto bw_hidden_state4 = model->addOperand(&type4);
  auto aux_input4 = model->addOperand(&type0);
  auto fw_aux_weights4 = model->addOperand(&type1);
  auto bw_aux_weights4 = model->addOperand(&type1);
  auto activation4 = model->addOperand(&type7);
  auto time_major4 = model->addOperand(&type8);
  auto merge_outputs4 = model->addOperand(&type8);
  auto fw_output4 = model->addOperand(&type6);
  auto bw_output3 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t activation4_init[] = {1};
  model->setOperandValue(activation4, activation4_init, sizeof(int32_t) * 1);
  static bool8 time_major4_init[] = {false};
  model->setOperandValue(time_major4, time_major4_init, sizeof(bool8) * 1);
  static bool8 merge_outputs4_init[] = {false};
  model->setOperandValue(merge_outputs4, merge_outputs4_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input4, fw_weights4, fw_recurrent_weights4, fw_bias4, fw_hidden_state4, bw_weights4, bw_recurrent_weights4, bw_bias4, bw_hidden_state4, aux_input4, fw_aux_weights4, bw_aux_weights4, activation4, time_major4, merge_outputs4}, {fw_output4, bw_output3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input4, fw_weights4, fw_recurrent_weights4, fw_bias4, fw_hidden_state4, bw_weights4, bw_recurrent_weights4, bw_bias4, bw_hidden_state4, aux_input4, fw_aux_weights4, bw_aux_weights4},
    {fw_output4, bw_output3});
  assert(model->isValid());
}

inline bool is_ignored_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_5(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 16, 8});
  OperandType type1(Type::TENSOR_FLOAT32, {16, 8});
  OperandType type2(Type::TENSOR_FLOAT32, {16, 16});
  OperandType type3(Type::TENSOR_FLOAT32, {16});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 16});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 16, 16});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto input4 = model->addOperand(&type0);
  auto fw_weights4 = model->addOperand(&type1);
  auto fw_recurrent_weights4 = model->addOperand(&type2);
  auto fw_bias4 = model->addOperand(&type3);
  auto fw_hidden_state4 = model->addOperand(&type4);
  auto bw_weights4 = model->addOperand(&type1);
  auto bw_recurrent_weights4 = model->addOperand(&type2);
  auto bw_bias4 = model->addOperand(&type3);
  auto bw_hidden_state4 = model->addOperand(&type4);
  auto aux_input4 = model->addOperand(&type0);
  auto fw_aux_weights4 = model->addOperand(&type1);
  auto bw_aux_weights4 = model->addOperand(&type1);
  auto activation4 = model->addOperand(&type7);
  auto time_major4 = model->addOperand(&type8);
  auto merge_outputs4 = model->addOperand(&type8);
  auto fw_output4 = model->addOperand(&type6);
  auto bw_output3 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t activation4_init[] = {1};
  model->setOperandValue(activation4, activation4_init, sizeof(int32_t) * 1);
  static bool8 time_major4_init[] = {false};
  model->setOperandValue(time_major4, time_major4_init, sizeof(bool8) * 1);
  static bool8 merge_outputs4_init[] = {false};
  model->setOperandValue(merge_outputs4, merge_outputs4_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input4, fw_weights4, fw_recurrent_weights4, fw_bias4, fw_hidden_state4, bw_weights4, bw_recurrent_weights4, bw_bias4, bw_hidden_state4, aux_input4, fw_aux_weights4, bw_aux_weights4, activation4, time_major4, merge_outputs4}, {fw_output4, bw_output3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input4, fw_weights4, fw_recurrent_weights4, fw_bias4, fw_hidden_state4, bw_weights4, bw_recurrent_weights4, bw_bias4, bw_hidden_state4, aux_input4, fw_aux_weights4, bw_aux_weights4},
    {fw_output4, bw_output3});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_5(Model *model) {
  OperandType type15(Type::TENSOR_FLOAT16, {16});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 16});
  OperandType type17(Type::TENSOR_FLOAT16, {2, 16, 16});
  OperandType type18(Type::TENSOR_FLOAT16, {16, 16});
  OperandType type19(Type::TENSOR_FLOAT16, {16, 8});
  OperandType type20(Type::TENSOR_FLOAT16, {2, 16, 8});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto input4 = model->addOperand(&type20);
  auto fw_weights4 = model->addOperand(&type19);
  auto fw_recurrent_weights4 = model->addOperand(&type18);
  auto fw_bias4 = model->addOperand(&type15);
  auto fw_hidden_state4 = model->addOperand(&type16);
  auto bw_weights4 = model->addOperand(&type19);
  auto bw_recurrent_weights4 = model->addOperand(&type18);
  auto bw_bias4 = model->addOperand(&type15);
  auto bw_hidden_state4 = model->addOperand(&type16);
  auto aux_input4 = model->addOperand(&type20);
  auto fw_aux_weights4 = model->addOperand(&type19);
  auto bw_aux_weights4 = model->addOperand(&type19);
  auto activation4 = model->addOperand(&type7);
  auto time_major4 = model->addOperand(&type8);
  auto merge_outputs4 = model->addOperand(&type8);
  auto fw_output4 = model->addOperand(&type17);
  auto bw_output3 = model->addOperand(&type17);
  // Phase 2, operations
  static int32_t activation4_init[] = {1};
  model->setOperandValue(activation4, activation4_init, sizeof(int32_t) * 1);
  static bool8 time_major4_init[] = {false};
  model->setOperandValue(time_major4, time_major4_init, sizeof(bool8) * 1);
  static bool8 merge_outputs4_init[] = {false};
  model->setOperandValue(merge_outputs4, merge_outputs4_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input4, fw_weights4, fw_recurrent_weights4, fw_bias4, fw_hidden_state4, bw_weights4, bw_recurrent_weights4, bw_bias4, bw_hidden_state4, aux_input4, fw_aux_weights4, bw_aux_weights4, activation4, time_major4, merge_outputs4}, {fw_output4, bw_output3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input4, fw_weights4, fw_recurrent_weights4, fw_bias4, fw_hidden_state4, bw_weights4, bw_recurrent_weights4, bw_bias4, bw_hidden_state4, aux_input4, fw_aux_weights4, bw_aux_weights4},
    {fw_output4, bw_output3});
  assert(model->isValid());
}

inline bool is_ignored_float16_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_5(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 16, 8});
  OperandType type1(Type::TENSOR_FLOAT32, {16, 8});
  OperandType type2(Type::TENSOR_FLOAT32, {16, 16});
  OperandType type21(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {16});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 16});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto input4 = model->addOperand(&type0);
  auto fw_weights4 = model->addOperand(&type1);
  auto fw_recurrent_weights4 = model->addOperand(&type2);
  auto fw_bias4 = model->addOperand(&type3);
  auto fw_hidden_state4 = model->addOperand(&type4);
  auto bw_weights4 = model->addOperand(&type1);
  auto bw_recurrent_weights4 = model->addOperand(&type2);
  auto bw_bias4 = model->addOperand(&type3);
  auto bw_hidden_state4 = model->addOperand(&type4);
  auto aux_input4 = model->addOperand(&type0);
  auto fw_aux_weights4 = model->addOperand(&type1);
  auto bw_aux_weights4 = model->addOperand(&type1);
  auto activation4 = model->addOperand(&type7);
  auto time_major4 = model->addOperand(&type8);
  auto merge_outputs4 = model->addOperand(&type8);
  auto fw_output4 = model->addOperand(&type21);
  auto bw_output3 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t activation4_init[] = {1};
  model->setOperandValue(activation4, activation4_init, sizeof(int32_t) * 1);
  static bool8 time_major4_init[] = {false};
  model->setOperandValue(time_major4, time_major4_init, sizeof(bool8) * 1);
  static bool8 merge_outputs4_init[] = {false};
  model->setOperandValue(merge_outputs4, merge_outputs4_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input4, fw_weights4, fw_recurrent_weights4, fw_bias4, fw_hidden_state4, bw_weights4, bw_recurrent_weights4, bw_bias4, bw_hidden_state4, aux_input4, fw_aux_weights4, bw_aux_weights4, activation4, time_major4, merge_outputs4}, {fw_output4, bw_output3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input4, fw_weights4, fw_recurrent_weights4, fw_bias4, fw_hidden_state4, bw_weights4, bw_recurrent_weights4, bw_bias4, bw_hidden_state4, aux_input4, fw_aux_weights4, bw_aux_weights4},
    {fw_output4, bw_output3});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_5(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 16, 8});
  OperandType type1(Type::TENSOR_FLOAT32, {16, 8});
  OperandType type2(Type::TENSOR_FLOAT32, {16, 16});
  OperandType type21(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {16});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 16});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto input4 = model->addOperand(&type0);
  auto fw_weights4 = model->addOperand(&type1);
  auto fw_recurrent_weights4 = model->addOperand(&type2);
  auto fw_bias4 = model->addOperand(&type3);
  auto fw_hidden_state4 = model->addOperand(&type4);
  auto bw_weights4 = model->addOperand(&type1);
  auto bw_recurrent_weights4 = model->addOperand(&type2);
  auto bw_bias4 = model->addOperand(&type3);
  auto bw_hidden_state4 = model->addOperand(&type4);
  auto aux_input4 = model->addOperand(&type0);
  auto fw_aux_weights4 = model->addOperand(&type1);
  auto bw_aux_weights4 = model->addOperand(&type1);
  auto activation4 = model->addOperand(&type7);
  auto time_major4 = model->addOperand(&type8);
  auto merge_outputs4 = model->addOperand(&type8);
  auto fw_output4 = model->addOperand(&type21);
  auto bw_output3 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t activation4_init[] = {1};
  model->setOperandValue(activation4, activation4_init, sizeof(int32_t) * 1);
  static bool8 time_major4_init[] = {false};
  model->setOperandValue(time_major4, time_major4_init, sizeof(bool8) * 1);
  static bool8 merge_outputs4_init[] = {false};
  model->setOperandValue(merge_outputs4, merge_outputs4_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input4, fw_weights4, fw_recurrent_weights4, fw_bias4, fw_hidden_state4, bw_weights4, bw_recurrent_weights4, bw_bias4, bw_hidden_state4, aux_input4, fw_aux_weights4, bw_aux_weights4, activation4, time_major4, merge_outputs4}, {fw_output4, bw_output3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input4, fw_weights4, fw_recurrent_weights4, fw_bias4, fw_hidden_state4, bw_weights4, bw_recurrent_weights4, bw_bias4, bw_hidden_state4, aux_input4, fw_aux_weights4, bw_aux_weights4},
    {fw_output4, bw_output3});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_5(Model *model) {
  OperandType type15(Type::TENSOR_FLOAT16, {16});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 16});
  OperandType type18(Type::TENSOR_FLOAT16, {16, 16});
  OperandType type19(Type::TENSOR_FLOAT16, {16, 8});
  OperandType type20(Type::TENSOR_FLOAT16, {2, 16, 8});
  OperandType type22(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto input4 = model->addOperand(&type20);
  auto fw_weights4 = model->addOperand(&type19);
  auto fw_recurrent_weights4 = model->addOperand(&type18);
  auto fw_bias4 = model->addOperand(&type15);
  auto fw_hidden_state4 = model->addOperand(&type16);
  auto bw_weights4 = model->addOperand(&type19);
  auto bw_recurrent_weights4 = model->addOperand(&type18);
  auto bw_bias4 = model->addOperand(&type15);
  auto bw_hidden_state4 = model->addOperand(&type16);
  auto aux_input4 = model->addOperand(&type20);
  auto fw_aux_weights4 = model->addOperand(&type19);
  auto bw_aux_weights4 = model->addOperand(&type19);
  auto activation4 = model->addOperand(&type7);
  auto time_major4 = model->addOperand(&type8);
  auto merge_outputs4 = model->addOperand(&type8);
  auto fw_output4 = model->addOperand(&type22);
  auto bw_output3 = model->addOperand(&type22);
  // Phase 2, operations
  static int32_t activation4_init[] = {1};
  model->setOperandValue(activation4, activation4_init, sizeof(int32_t) * 1);
  static bool8 time_major4_init[] = {false};
  model->setOperandValue(time_major4, time_major4_init, sizeof(bool8) * 1);
  static bool8 merge_outputs4_init[] = {false};
  model->setOperandValue(merge_outputs4, merge_outputs4_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input4, fw_weights4, fw_recurrent_weights4, fw_bias4, fw_hidden_state4, bw_weights4, bw_recurrent_weights4, bw_bias4, bw_hidden_state4, aux_input4, fw_aux_weights4, bw_aux_weights4, activation4, time_major4, merge_outputs4}, {fw_output4, bw_output3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input4, fw_weights4, fw_recurrent_weights4, fw_bias4, fw_hidden_state4, bw_weights4, bw_recurrent_weights4, bw_bias4, bw_hidden_state4, aux_input4, fw_aux_weights4, bw_aux_weights4},
    {fw_output4, bw_output3});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_6(Model *model) {
  OperandType type12(Type::TENSOR_FLOAT32, {2, 16, 4});
  OperandType type13(Type::TENSOR_FLOAT32, {16, 4});
  OperandType type2(Type::TENSOR_FLOAT32, {16, 16});
  OperandType type3(Type::TENSOR_FLOAT32, {16});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 16});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 16, 16});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto input5 = model->addOperand(&type12);
  auto fw_weights5 = model->addOperand(&type13);
  auto fw_recurrent_weights5 = model->addOperand(&type2);
  auto fw_bias5 = model->addOperand(&type3);
  auto fw_hidden_state5 = model->addOperand(&type4);
  auto bw_weights5 = model->addOperand(&type13);
  auto bw_recurrent_weights5 = model->addOperand(&type2);
  auto bw_bias5 = model->addOperand(&type3);
  auto bw_hidden_state5 = model->addOperand(&type4);
  auto aux_input5 = model->addOperand(&type12);
  auto fw_aux_weights5 = model->addOperand(&type13);
  auto bw_aux_weights5 = model->addOperand(&type13);
  auto activation5 = model->addOperand(&type7);
  auto time_major5 = model->addOperand(&type8);
  auto merge_outputs5 = model->addOperand(&type8);
  auto fw_output5 = model->addOperand(&type6);
  auto bw_output4 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t activation5_init[] = {1};
  model->setOperandValue(activation5, activation5_init, sizeof(int32_t) * 1);
  static bool8 time_major5_init[] = {false};
  model->setOperandValue(time_major5, time_major5_init, sizeof(bool8) * 1);
  static bool8 merge_outputs5_init[] = {false};
  model->setOperandValue(merge_outputs5, merge_outputs5_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input5, fw_weights5, fw_recurrent_weights5, fw_bias5, fw_hidden_state5, bw_weights5, bw_recurrent_weights5, bw_bias5, bw_hidden_state5, aux_input5, fw_aux_weights5, bw_aux_weights5, activation5, time_major5, merge_outputs5}, {fw_output5, bw_output4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input5, fw_weights5, fw_recurrent_weights5, fw_bias5, fw_hidden_state5, bw_weights5, bw_recurrent_weights5, bw_bias5, bw_hidden_state5, aux_input5, fw_aux_weights5, bw_aux_weights5},
    {fw_output5, bw_output4});
  assert(model->isValid());
}

inline bool is_ignored_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_6(Model *model) {
  OperandType type12(Type::TENSOR_FLOAT32, {2, 16, 4});
  OperandType type13(Type::TENSOR_FLOAT32, {16, 4});
  OperandType type2(Type::TENSOR_FLOAT32, {16, 16});
  OperandType type3(Type::TENSOR_FLOAT32, {16});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 16});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 16, 16});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto input5 = model->addOperand(&type12);
  auto fw_weights5 = model->addOperand(&type13);
  auto fw_recurrent_weights5 = model->addOperand(&type2);
  auto fw_bias5 = model->addOperand(&type3);
  auto fw_hidden_state5 = model->addOperand(&type4);
  auto bw_weights5 = model->addOperand(&type13);
  auto bw_recurrent_weights5 = model->addOperand(&type2);
  auto bw_bias5 = model->addOperand(&type3);
  auto bw_hidden_state5 = model->addOperand(&type4);
  auto aux_input5 = model->addOperand(&type12);
  auto fw_aux_weights5 = model->addOperand(&type13);
  auto bw_aux_weights5 = model->addOperand(&type13);
  auto activation5 = model->addOperand(&type7);
  auto time_major5 = model->addOperand(&type8);
  auto merge_outputs5 = model->addOperand(&type8);
  auto fw_output5 = model->addOperand(&type6);
  auto bw_output4 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t activation5_init[] = {1};
  model->setOperandValue(activation5, activation5_init, sizeof(int32_t) * 1);
  static bool8 time_major5_init[] = {false};
  model->setOperandValue(time_major5, time_major5_init, sizeof(bool8) * 1);
  static bool8 merge_outputs5_init[] = {false};
  model->setOperandValue(merge_outputs5, merge_outputs5_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input5, fw_weights5, fw_recurrent_weights5, fw_bias5, fw_hidden_state5, bw_weights5, bw_recurrent_weights5, bw_bias5, bw_hidden_state5, aux_input5, fw_aux_weights5, bw_aux_weights5, activation5, time_major5, merge_outputs5}, {fw_output5, bw_output4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input5, fw_weights5, fw_recurrent_weights5, fw_bias5, fw_hidden_state5, bw_weights5, bw_recurrent_weights5, bw_bias5, bw_hidden_state5, aux_input5, fw_aux_weights5, bw_aux_weights5},
    {fw_output5, bw_output4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_6(Model *model) {
  OperandType type15(Type::TENSOR_FLOAT16, {16});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 16});
  OperandType type17(Type::TENSOR_FLOAT16, {2, 16, 16});
  OperandType type18(Type::TENSOR_FLOAT16, {16, 16});
  OperandType type26(Type::TENSOR_FLOAT16, {2, 16, 4});
  OperandType type27(Type::TENSOR_FLOAT16, {16, 4});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto input5 = model->addOperand(&type26);
  auto fw_weights5 = model->addOperand(&type27);
  auto fw_recurrent_weights5 = model->addOperand(&type18);
  auto fw_bias5 = model->addOperand(&type15);
  auto fw_hidden_state5 = model->addOperand(&type16);
  auto bw_weights5 = model->addOperand(&type27);
  auto bw_recurrent_weights5 = model->addOperand(&type18);
  auto bw_bias5 = model->addOperand(&type15);
  auto bw_hidden_state5 = model->addOperand(&type16);
  auto aux_input5 = model->addOperand(&type26);
  auto fw_aux_weights5 = model->addOperand(&type27);
  auto bw_aux_weights5 = model->addOperand(&type27);
  auto activation5 = model->addOperand(&type7);
  auto time_major5 = model->addOperand(&type8);
  auto merge_outputs5 = model->addOperand(&type8);
  auto fw_output5 = model->addOperand(&type17);
  auto bw_output4 = model->addOperand(&type17);
  // Phase 2, operations
  static int32_t activation5_init[] = {1};
  model->setOperandValue(activation5, activation5_init, sizeof(int32_t) * 1);
  static bool8 time_major5_init[] = {false};
  model->setOperandValue(time_major5, time_major5_init, sizeof(bool8) * 1);
  static bool8 merge_outputs5_init[] = {false};
  model->setOperandValue(merge_outputs5, merge_outputs5_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input5, fw_weights5, fw_recurrent_weights5, fw_bias5, fw_hidden_state5, bw_weights5, bw_recurrent_weights5, bw_bias5, bw_hidden_state5, aux_input5, fw_aux_weights5, bw_aux_weights5, activation5, time_major5, merge_outputs5}, {fw_output5, bw_output4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input5, fw_weights5, fw_recurrent_weights5, fw_bias5, fw_hidden_state5, bw_weights5, bw_recurrent_weights5, bw_bias5, bw_hidden_state5, aux_input5, fw_aux_weights5, bw_aux_weights5},
    {fw_output5, bw_output4});
  assert(model->isValid());
}

inline bool is_ignored_float16_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_6(Model *model) {
  OperandType type12(Type::TENSOR_FLOAT32, {2, 16, 4});
  OperandType type13(Type::TENSOR_FLOAT32, {16, 4});
  OperandType type2(Type::TENSOR_FLOAT32, {16, 16});
  OperandType type21(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {16});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 16});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto input5 = model->addOperand(&type12);
  auto fw_weights5 = model->addOperand(&type13);
  auto fw_recurrent_weights5 = model->addOperand(&type2);
  auto fw_bias5 = model->addOperand(&type3);
  auto fw_hidden_state5 = model->addOperand(&type4);
  auto bw_weights5 = model->addOperand(&type13);
  auto bw_recurrent_weights5 = model->addOperand(&type2);
  auto bw_bias5 = model->addOperand(&type3);
  auto bw_hidden_state5 = model->addOperand(&type4);
  auto aux_input5 = model->addOperand(&type12);
  auto fw_aux_weights5 = model->addOperand(&type13);
  auto bw_aux_weights5 = model->addOperand(&type13);
  auto activation5 = model->addOperand(&type7);
  auto time_major5 = model->addOperand(&type8);
  auto merge_outputs5 = model->addOperand(&type8);
  auto fw_output5 = model->addOperand(&type21);
  auto bw_output4 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t activation5_init[] = {1};
  model->setOperandValue(activation5, activation5_init, sizeof(int32_t) * 1);
  static bool8 time_major5_init[] = {false};
  model->setOperandValue(time_major5, time_major5_init, sizeof(bool8) * 1);
  static bool8 merge_outputs5_init[] = {false};
  model->setOperandValue(merge_outputs5, merge_outputs5_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input5, fw_weights5, fw_recurrent_weights5, fw_bias5, fw_hidden_state5, bw_weights5, bw_recurrent_weights5, bw_bias5, bw_hidden_state5, aux_input5, fw_aux_weights5, bw_aux_weights5, activation5, time_major5, merge_outputs5}, {fw_output5, bw_output4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input5, fw_weights5, fw_recurrent_weights5, fw_bias5, fw_hidden_state5, bw_weights5, bw_recurrent_weights5, bw_bias5, bw_hidden_state5, aux_input5, fw_aux_weights5, bw_aux_weights5},
    {fw_output5, bw_output4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_6(Model *model) {
  OperandType type12(Type::TENSOR_FLOAT32, {2, 16, 4});
  OperandType type13(Type::TENSOR_FLOAT32, {16, 4});
  OperandType type2(Type::TENSOR_FLOAT32, {16, 16});
  OperandType type21(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {16});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 16});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto input5 = model->addOperand(&type12);
  auto fw_weights5 = model->addOperand(&type13);
  auto fw_recurrent_weights5 = model->addOperand(&type2);
  auto fw_bias5 = model->addOperand(&type3);
  auto fw_hidden_state5 = model->addOperand(&type4);
  auto bw_weights5 = model->addOperand(&type13);
  auto bw_recurrent_weights5 = model->addOperand(&type2);
  auto bw_bias5 = model->addOperand(&type3);
  auto bw_hidden_state5 = model->addOperand(&type4);
  auto aux_input5 = model->addOperand(&type12);
  auto fw_aux_weights5 = model->addOperand(&type13);
  auto bw_aux_weights5 = model->addOperand(&type13);
  auto activation5 = model->addOperand(&type7);
  auto time_major5 = model->addOperand(&type8);
  auto merge_outputs5 = model->addOperand(&type8);
  auto fw_output5 = model->addOperand(&type21);
  auto bw_output4 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t activation5_init[] = {1};
  model->setOperandValue(activation5, activation5_init, sizeof(int32_t) * 1);
  static bool8 time_major5_init[] = {false};
  model->setOperandValue(time_major5, time_major5_init, sizeof(bool8) * 1);
  static bool8 merge_outputs5_init[] = {false};
  model->setOperandValue(merge_outputs5, merge_outputs5_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input5, fw_weights5, fw_recurrent_weights5, fw_bias5, fw_hidden_state5, bw_weights5, bw_recurrent_weights5, bw_bias5, bw_hidden_state5, aux_input5, fw_aux_weights5, bw_aux_weights5, activation5, time_major5, merge_outputs5}, {fw_output5, bw_output4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input5, fw_weights5, fw_recurrent_weights5, fw_bias5, fw_hidden_state5, bw_weights5, bw_recurrent_weights5, bw_bias5, bw_hidden_state5, aux_input5, fw_aux_weights5, bw_aux_weights5},
    {fw_output5, bw_output4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_6(Model *model) {
  OperandType type15(Type::TENSOR_FLOAT16, {16});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 16});
  OperandType type18(Type::TENSOR_FLOAT16, {16, 16});
  OperandType type22(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type26(Type::TENSOR_FLOAT16, {2, 16, 4});
  OperandType type27(Type::TENSOR_FLOAT16, {16, 4});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto input5 = model->addOperand(&type26);
  auto fw_weights5 = model->addOperand(&type27);
  auto fw_recurrent_weights5 = model->addOperand(&type18);
  auto fw_bias5 = model->addOperand(&type15);
  auto fw_hidden_state5 = model->addOperand(&type16);
  auto bw_weights5 = model->addOperand(&type27);
  auto bw_recurrent_weights5 = model->addOperand(&type18);
  auto bw_bias5 = model->addOperand(&type15);
  auto bw_hidden_state5 = model->addOperand(&type16);
  auto aux_input5 = model->addOperand(&type26);
  auto fw_aux_weights5 = model->addOperand(&type27);
  auto bw_aux_weights5 = model->addOperand(&type27);
  auto activation5 = model->addOperand(&type7);
  auto time_major5 = model->addOperand(&type8);
  auto merge_outputs5 = model->addOperand(&type8);
  auto fw_output5 = model->addOperand(&type22);
  auto bw_output4 = model->addOperand(&type22);
  // Phase 2, operations
  static int32_t activation5_init[] = {1};
  model->setOperandValue(activation5, activation5_init, sizeof(int32_t) * 1);
  static bool8 time_major5_init[] = {false};
  model->setOperandValue(time_major5, time_major5_init, sizeof(bool8) * 1);
  static bool8 merge_outputs5_init[] = {false};
  model->setOperandValue(merge_outputs5, merge_outputs5_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_RNN, {input5, fw_weights5, fw_recurrent_weights5, fw_bias5, fw_hidden_state5, bw_weights5, bw_recurrent_weights5, bw_bias5, bw_hidden_state5, aux_input5, fw_aux_weights5, bw_aux_weights5, activation5, time_major5, merge_outputs5}, {fw_output5, bw_output4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input5, fw_weights5, fw_recurrent_weights5, fw_bias5, fw_hidden_state5, bw_weights5, bw_recurrent_weights5, bw_bias5, bw_hidden_state5, aux_input5, fw_aux_weights5, bw_aux_weights5},
    {fw_output5, bw_output4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

