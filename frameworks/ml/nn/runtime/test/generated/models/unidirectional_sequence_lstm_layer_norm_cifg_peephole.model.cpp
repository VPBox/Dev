// clang-format off
// Generated file (from: unidirectional_sequence_lstm_layer_norm_cifg_peephole.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {3, 2, 5});
  OperandType type1(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type10(Type::FLOAT32, {});
  OperandType type11(Type::BOOL, {});
  OperandType type12(Type::TENSOR_FLOAT32, {3, 2, 3});
  OperandType type2(Type::TENSOR_FLOAT32, {4, 5});
  OperandType type3(Type::TENSOR_FLOAT32, {4, 3});
  OperandType type4(Type::TENSOR_FLOAT32, {0});
  OperandType type5(Type::TENSOR_FLOAT32, {4});
  OperandType type6(Type::TENSOR_FLOAT32, {3, 4});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {2, 4});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto input_to_input_weights = model->addOperand(&type1);
  auto input_to_forget_weights = model->addOperand(&type2);
  auto input_to_cell_weights = model->addOperand(&type2);
  auto input_to_output_weights = model->addOperand(&type2);
  auto recurrent_to_intput_weights = model->addOperand(&type1);
  auto recurrent_to_forget_weights = model->addOperand(&type3);
  auto recurrent_to_cell_weights = model->addOperand(&type3);
  auto recurrent_to_output_weights = model->addOperand(&type3);
  auto cell_to_input_weights = model->addOperand(&type4);
  auto cell_to_forget_weights = model->addOperand(&type5);
  auto cell_to_output_weights = model->addOperand(&type5);
  auto input_gate_bias = model->addOperand(&type4);
  auto forget_gate_bias = model->addOperand(&type5);
  auto cell_gate_bias = model->addOperand(&type5);
  auto output_gate_bias = model->addOperand(&type5);
  auto projection_weights = model->addOperand(&type6);
  auto projection_bias = model->addOperand(&type4);
  auto output_state_in = model->addOperand(&type7);
  auto cell_state_in = model->addOperand(&type8);
  auto activation_param = model->addOperand(&type9);
  auto cell_clip_param = model->addOperand(&type10);
  auto proj_clip_param = model->addOperand(&type10);
  auto time_major_param = model->addOperand(&type11);
  auto input_layer_norm_weights = model->addOperand(&type4);
  auto forget_layer_norm_weights = model->addOperand(&type5);
  auto cell_layer_norm_weights = model->addOperand(&type5);
  auto output_layer_norm_weights = model->addOperand(&type5);
  auto output = model->addOperand(&type12);
  // Phase 2, operations
  static int32_t activation_param_init[] = {4};
  model->setOperandValue(activation_param, activation_param_init, sizeof(int32_t) * 1);
  static float cell_clip_param_init[] = {0.0f};
  model->setOperandValue(cell_clip_param, cell_clip_param_init, sizeof(float) * 1);
  static float proj_clip_param_init[] = {0.0f};
  model->setOperandValue(proj_clip_param, proj_clip_param_init, sizeof(float) * 1);
  static bool8 time_major_param_init[] = {true};
  model->setOperandValue(time_major_param, time_major_param_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_UNIDIRECTIONAL_SEQUENCE_LSTM, {input, input_to_input_weights, input_to_forget_weights, input_to_cell_weights, input_to_output_weights, recurrent_to_intput_weights, recurrent_to_forget_weights, recurrent_to_cell_weights, recurrent_to_output_weights, cell_to_input_weights, cell_to_forget_weights, cell_to_output_weights, input_gate_bias, forget_gate_bias, cell_gate_bias, output_gate_bias, projection_weights, projection_bias, output_state_in, cell_state_in, activation_param, cell_clip_param, proj_clip_param, time_major_param, input_layer_norm_weights, forget_layer_norm_weights, cell_layer_norm_weights, output_layer_norm_weights}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, input_to_input_weights, input_to_forget_weights, input_to_cell_weights, input_to_output_weights, recurrent_to_intput_weights, recurrent_to_forget_weights, recurrent_to_cell_weights, recurrent_to_output_weights, cell_to_input_weights, cell_to_forget_weights, cell_to_output_weights, input_gate_bias, forget_gate_bias, cell_gate_bias, output_gate_bias, projection_weights, projection_bias, output_state_in, cell_state_in, input_layer_norm_weights, forget_layer_norm_weights, cell_layer_norm_weights, output_layer_norm_weights},
    {output});
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {3, 2, 5});
  OperandType type1(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type10(Type::FLOAT32, {});
  OperandType type11(Type::BOOL, {});
  OperandType type12(Type::TENSOR_FLOAT32, {3, 2, 3});
  OperandType type2(Type::TENSOR_FLOAT32, {4, 5});
  OperandType type3(Type::TENSOR_FLOAT32, {4, 3});
  OperandType type4(Type::TENSOR_FLOAT32, {0});
  OperandType type5(Type::TENSOR_FLOAT32, {4});
  OperandType type6(Type::TENSOR_FLOAT32, {3, 4});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {2, 4});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto input_to_input_weights = model->addOperand(&type1);
  auto input_to_forget_weights = model->addOperand(&type2);
  auto input_to_cell_weights = model->addOperand(&type2);
  auto input_to_output_weights = model->addOperand(&type2);
  auto recurrent_to_intput_weights = model->addOperand(&type1);
  auto recurrent_to_forget_weights = model->addOperand(&type3);
  auto recurrent_to_cell_weights = model->addOperand(&type3);
  auto recurrent_to_output_weights = model->addOperand(&type3);
  auto cell_to_input_weights = model->addOperand(&type4);
  auto cell_to_forget_weights = model->addOperand(&type5);
  auto cell_to_output_weights = model->addOperand(&type5);
  auto input_gate_bias = model->addOperand(&type4);
  auto forget_gate_bias = model->addOperand(&type5);
  auto cell_gate_bias = model->addOperand(&type5);
  auto output_gate_bias = model->addOperand(&type5);
  auto projection_weights = model->addOperand(&type6);
  auto projection_bias = model->addOperand(&type4);
  auto output_state_in = model->addOperand(&type7);
  auto cell_state_in = model->addOperand(&type8);
  auto activation_param = model->addOperand(&type9);
  auto cell_clip_param = model->addOperand(&type10);
  auto proj_clip_param = model->addOperand(&type10);
  auto time_major_param = model->addOperand(&type11);
  auto input_layer_norm_weights = model->addOperand(&type4);
  auto forget_layer_norm_weights = model->addOperand(&type5);
  auto cell_layer_norm_weights = model->addOperand(&type5);
  auto output_layer_norm_weights = model->addOperand(&type5);
  auto output = model->addOperand(&type12);
  // Phase 2, operations
  static int32_t activation_param_init[] = {4};
  model->setOperandValue(activation_param, activation_param_init, sizeof(int32_t) * 1);
  static float cell_clip_param_init[] = {0.0f};
  model->setOperandValue(cell_clip_param, cell_clip_param_init, sizeof(float) * 1);
  static float proj_clip_param_init[] = {0.0f};
  model->setOperandValue(proj_clip_param, proj_clip_param_init, sizeof(float) * 1);
  static bool8 time_major_param_init[] = {true};
  model->setOperandValue(time_major_param, time_major_param_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_UNIDIRECTIONAL_SEQUENCE_LSTM, {input, input_to_input_weights, input_to_forget_weights, input_to_cell_weights, input_to_output_weights, recurrent_to_intput_weights, recurrent_to_forget_weights, recurrent_to_cell_weights, recurrent_to_output_weights, cell_to_input_weights, cell_to_forget_weights, cell_to_output_weights, input_gate_bias, forget_gate_bias, cell_gate_bias, output_gate_bias, projection_weights, projection_bias, output_state_in, cell_state_in, activation_param, cell_clip_param, proj_clip_param, time_major_param, input_layer_norm_weights, forget_layer_norm_weights, cell_layer_norm_weights, output_layer_norm_weights}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, input_to_input_weights, input_to_forget_weights, input_to_cell_weights, input_to_output_weights, recurrent_to_intput_weights, recurrent_to_forget_weights, recurrent_to_cell_weights, recurrent_to_output_weights, cell_to_input_weights, cell_to_forget_weights, cell_to_output_weights, input_gate_bias, forget_gate_bias, cell_gate_bias, output_gate_bias, projection_weights, projection_bias, output_state_in, cell_state_in, input_layer_norm_weights, forget_layer_norm_weights, cell_layer_norm_weights, output_layer_norm_weights},
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
  OperandType type11(Type::BOOL, {});
  OperandType type13(Type::FLOAT16, {});
  OperandType type14(Type::TENSOR_FLOAT16, {4});
  OperandType type15(Type::TENSOR_FLOAT16, {2, 4});
  OperandType type16(Type::TENSOR_FLOAT16, {0});
  OperandType type17(Type::TENSOR_FLOAT16, {3, 2, 5});
  OperandType type18(Type::TENSOR_FLOAT16, {4, 5});
  OperandType type19(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type20(Type::TENSOR_FLOAT16, {3, 2, 3});
  OperandType type21(Type::TENSOR_FLOAT16, {2, 3});
  OperandType type22(Type::TENSOR_FLOAT16, {3, 4});
  OperandType type23(Type::TENSOR_FLOAT16, {4, 3});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto input = model->addOperand(&type17);
  auto input_to_input_weights = model->addOperand(&type19);
  auto input_to_forget_weights = model->addOperand(&type18);
  auto input_to_cell_weights = model->addOperand(&type18);
  auto input_to_output_weights = model->addOperand(&type18);
  auto recurrent_to_intput_weights = model->addOperand(&type19);
  auto recurrent_to_forget_weights = model->addOperand(&type23);
  auto recurrent_to_cell_weights = model->addOperand(&type23);
  auto recurrent_to_output_weights = model->addOperand(&type23);
  auto cell_to_input_weights = model->addOperand(&type16);
  auto cell_to_forget_weights = model->addOperand(&type14);
  auto cell_to_output_weights = model->addOperand(&type14);
  auto input_gate_bias = model->addOperand(&type16);
  auto forget_gate_bias = model->addOperand(&type14);
  auto cell_gate_bias = model->addOperand(&type14);
  auto output_gate_bias = model->addOperand(&type14);
  auto projection_weights = model->addOperand(&type22);
  auto projection_bias = model->addOperand(&type16);
  auto output_state_in = model->addOperand(&type21);
  auto cell_state_in = model->addOperand(&type15);
  auto activation_param = model->addOperand(&type9);
  auto cell_clip_param = model->addOperand(&type13);
  auto proj_clip_param = model->addOperand(&type13);
  auto time_major_param = model->addOperand(&type11);
  auto input_layer_norm_weights = model->addOperand(&type16);
  auto forget_layer_norm_weights = model->addOperand(&type14);
  auto cell_layer_norm_weights = model->addOperand(&type14);
  auto output_layer_norm_weights = model->addOperand(&type14);
  auto output = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t activation_param_init[] = {4};
  model->setOperandValue(activation_param, activation_param_init, sizeof(int32_t) * 1);
  static _Float16 cell_clip_param_init[] = {0.0f};
  model->setOperandValue(cell_clip_param, cell_clip_param_init, sizeof(_Float16) * 1);
  static _Float16 proj_clip_param_init[] = {0.0f};
  model->setOperandValue(proj_clip_param, proj_clip_param_init, sizeof(_Float16) * 1);
  static bool8 time_major_param_init[] = {true};
  model->setOperandValue(time_major_param, time_major_param_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_UNIDIRECTIONAL_SEQUENCE_LSTM, {input, input_to_input_weights, input_to_forget_weights, input_to_cell_weights, input_to_output_weights, recurrent_to_intput_weights, recurrent_to_forget_weights, recurrent_to_cell_weights, recurrent_to_output_weights, cell_to_input_weights, cell_to_forget_weights, cell_to_output_weights, input_gate_bias, forget_gate_bias, cell_gate_bias, output_gate_bias, projection_weights, projection_bias, output_state_in, cell_state_in, activation_param, cell_clip_param, proj_clip_param, time_major_param, input_layer_norm_weights, forget_layer_norm_weights, cell_layer_norm_weights, output_layer_norm_weights}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, input_to_input_weights, input_to_forget_weights, input_to_cell_weights, input_to_output_weights, recurrent_to_intput_weights, recurrent_to_forget_weights, recurrent_to_cell_weights, recurrent_to_output_weights, cell_to_input_weights, cell_to_forget_weights, cell_to_output_weights, input_gate_bias, forget_gate_bias, cell_gate_bias, output_gate_bias, projection_weights, projection_bias, output_state_in, cell_state_in, input_layer_norm_weights, forget_layer_norm_weights, cell_layer_norm_weights, output_layer_norm_weights},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {3, 2, 5});
  OperandType type1(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type10(Type::FLOAT32, {});
  OperandType type11(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {4, 5});
  OperandType type24(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {4, 3});
  OperandType type4(Type::TENSOR_FLOAT32, {0});
  OperandType type5(Type::TENSOR_FLOAT32, {4});
  OperandType type6(Type::TENSOR_FLOAT32, {3, 4});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {2, 4});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto input_to_input_weights = model->addOperand(&type1);
  auto input_to_forget_weights = model->addOperand(&type2);
  auto input_to_cell_weights = model->addOperand(&type2);
  auto input_to_output_weights = model->addOperand(&type2);
  auto recurrent_to_intput_weights = model->addOperand(&type1);
  auto recurrent_to_forget_weights = model->addOperand(&type3);
  auto recurrent_to_cell_weights = model->addOperand(&type3);
  auto recurrent_to_output_weights = model->addOperand(&type3);
  auto cell_to_input_weights = model->addOperand(&type4);
  auto cell_to_forget_weights = model->addOperand(&type5);
  auto cell_to_output_weights = model->addOperand(&type5);
  auto input_gate_bias = model->addOperand(&type4);
  auto forget_gate_bias = model->addOperand(&type5);
  auto cell_gate_bias = model->addOperand(&type5);
  auto output_gate_bias = model->addOperand(&type5);
  auto projection_weights = model->addOperand(&type6);
  auto projection_bias = model->addOperand(&type4);
  auto output_state_in = model->addOperand(&type7);
  auto cell_state_in = model->addOperand(&type8);
  auto activation_param = model->addOperand(&type9);
  auto cell_clip_param = model->addOperand(&type10);
  auto proj_clip_param = model->addOperand(&type10);
  auto time_major_param = model->addOperand(&type11);
  auto input_layer_norm_weights = model->addOperand(&type4);
  auto forget_layer_norm_weights = model->addOperand(&type5);
  auto cell_layer_norm_weights = model->addOperand(&type5);
  auto output_layer_norm_weights = model->addOperand(&type5);
  auto output = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t activation_param_init[] = {4};
  model->setOperandValue(activation_param, activation_param_init, sizeof(int32_t) * 1);
  static float cell_clip_param_init[] = {0.0f};
  model->setOperandValue(cell_clip_param, cell_clip_param_init, sizeof(float) * 1);
  static float proj_clip_param_init[] = {0.0f};
  model->setOperandValue(proj_clip_param, proj_clip_param_init, sizeof(float) * 1);
  static bool8 time_major_param_init[] = {true};
  model->setOperandValue(time_major_param, time_major_param_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_UNIDIRECTIONAL_SEQUENCE_LSTM, {input, input_to_input_weights, input_to_forget_weights, input_to_cell_weights, input_to_output_weights, recurrent_to_intput_weights, recurrent_to_forget_weights, recurrent_to_cell_weights, recurrent_to_output_weights, cell_to_input_weights, cell_to_forget_weights, cell_to_output_weights, input_gate_bias, forget_gate_bias, cell_gate_bias, output_gate_bias, projection_weights, projection_bias, output_state_in, cell_state_in, activation_param, cell_clip_param, proj_clip_param, time_major_param, input_layer_norm_weights, forget_layer_norm_weights, cell_layer_norm_weights, output_layer_norm_weights}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, input_to_input_weights, input_to_forget_weights, input_to_cell_weights, input_to_output_weights, recurrent_to_intput_weights, recurrent_to_forget_weights, recurrent_to_cell_weights, recurrent_to_output_weights, cell_to_input_weights, cell_to_forget_weights, cell_to_output_weights, input_gate_bias, forget_gate_bias, cell_gate_bias, output_gate_bias, projection_weights, projection_bias, output_state_in, cell_state_in, input_layer_norm_weights, forget_layer_norm_weights, cell_layer_norm_weights, output_layer_norm_weights},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {3, 2, 5});
  OperandType type1(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type10(Type::FLOAT32, {});
  OperandType type11(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {4, 5});
  OperandType type24(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {4, 3});
  OperandType type4(Type::TENSOR_FLOAT32, {0});
  OperandType type5(Type::TENSOR_FLOAT32, {4});
  OperandType type6(Type::TENSOR_FLOAT32, {3, 4});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {2, 4});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto input_to_input_weights = model->addOperand(&type1);
  auto input_to_forget_weights = model->addOperand(&type2);
  auto input_to_cell_weights = model->addOperand(&type2);
  auto input_to_output_weights = model->addOperand(&type2);
  auto recurrent_to_intput_weights = model->addOperand(&type1);
  auto recurrent_to_forget_weights = model->addOperand(&type3);
  auto recurrent_to_cell_weights = model->addOperand(&type3);
  auto recurrent_to_output_weights = model->addOperand(&type3);
  auto cell_to_input_weights = model->addOperand(&type4);
  auto cell_to_forget_weights = model->addOperand(&type5);
  auto cell_to_output_weights = model->addOperand(&type5);
  auto input_gate_bias = model->addOperand(&type4);
  auto forget_gate_bias = model->addOperand(&type5);
  auto cell_gate_bias = model->addOperand(&type5);
  auto output_gate_bias = model->addOperand(&type5);
  auto projection_weights = model->addOperand(&type6);
  auto projection_bias = model->addOperand(&type4);
  auto output_state_in = model->addOperand(&type7);
  auto cell_state_in = model->addOperand(&type8);
  auto activation_param = model->addOperand(&type9);
  auto cell_clip_param = model->addOperand(&type10);
  auto proj_clip_param = model->addOperand(&type10);
  auto time_major_param = model->addOperand(&type11);
  auto input_layer_norm_weights = model->addOperand(&type4);
  auto forget_layer_norm_weights = model->addOperand(&type5);
  auto cell_layer_norm_weights = model->addOperand(&type5);
  auto output_layer_norm_weights = model->addOperand(&type5);
  auto output = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t activation_param_init[] = {4};
  model->setOperandValue(activation_param, activation_param_init, sizeof(int32_t) * 1);
  static float cell_clip_param_init[] = {0.0f};
  model->setOperandValue(cell_clip_param, cell_clip_param_init, sizeof(float) * 1);
  static float proj_clip_param_init[] = {0.0f};
  model->setOperandValue(proj_clip_param, proj_clip_param_init, sizeof(float) * 1);
  static bool8 time_major_param_init[] = {true};
  model->setOperandValue(time_major_param, time_major_param_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_UNIDIRECTIONAL_SEQUENCE_LSTM, {input, input_to_input_weights, input_to_forget_weights, input_to_cell_weights, input_to_output_weights, recurrent_to_intput_weights, recurrent_to_forget_weights, recurrent_to_cell_weights, recurrent_to_output_weights, cell_to_input_weights, cell_to_forget_weights, cell_to_output_weights, input_gate_bias, forget_gate_bias, cell_gate_bias, output_gate_bias, projection_weights, projection_bias, output_state_in, cell_state_in, activation_param, cell_clip_param, proj_clip_param, time_major_param, input_layer_norm_weights, forget_layer_norm_weights, cell_layer_norm_weights, output_layer_norm_weights}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, input_to_input_weights, input_to_forget_weights, input_to_cell_weights, input_to_output_weights, recurrent_to_intput_weights, recurrent_to_forget_weights, recurrent_to_cell_weights, recurrent_to_output_weights, cell_to_input_weights, cell_to_forget_weights, cell_to_output_weights, input_gate_bias, forget_gate_bias, cell_gate_bias, output_gate_bias, projection_weights, projection_bias, output_state_in, cell_state_in, input_layer_norm_weights, forget_layer_norm_weights, cell_layer_norm_weights, output_layer_norm_weights},
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
  OperandType type11(Type::BOOL, {});
  OperandType type13(Type::FLOAT16, {});
  OperandType type14(Type::TENSOR_FLOAT16, {4});
  OperandType type15(Type::TENSOR_FLOAT16, {2, 4});
  OperandType type16(Type::TENSOR_FLOAT16, {0});
  OperandType type17(Type::TENSOR_FLOAT16, {3, 2, 5});
  OperandType type18(Type::TENSOR_FLOAT16, {4, 5});
  OperandType type19(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type21(Type::TENSOR_FLOAT16, {2, 3});
  OperandType type22(Type::TENSOR_FLOAT16, {3, 4});
  OperandType type23(Type::TENSOR_FLOAT16, {4, 3});
  OperandType type25(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type9(Type::INT32, {});
  // Phase 1, operands
  auto input = model->addOperand(&type17);
  auto input_to_input_weights = model->addOperand(&type19);
  auto input_to_forget_weights = model->addOperand(&type18);
  auto input_to_cell_weights = model->addOperand(&type18);
  auto input_to_output_weights = model->addOperand(&type18);
  auto recurrent_to_intput_weights = model->addOperand(&type19);
  auto recurrent_to_forget_weights = model->addOperand(&type23);
  auto recurrent_to_cell_weights = model->addOperand(&type23);
  auto recurrent_to_output_weights = model->addOperand(&type23);
  auto cell_to_input_weights = model->addOperand(&type16);
  auto cell_to_forget_weights = model->addOperand(&type14);
  auto cell_to_output_weights = model->addOperand(&type14);
  auto input_gate_bias = model->addOperand(&type16);
  auto forget_gate_bias = model->addOperand(&type14);
  auto cell_gate_bias = model->addOperand(&type14);
  auto output_gate_bias = model->addOperand(&type14);
  auto projection_weights = model->addOperand(&type22);
  auto projection_bias = model->addOperand(&type16);
  auto output_state_in = model->addOperand(&type21);
  auto cell_state_in = model->addOperand(&type15);
  auto activation_param = model->addOperand(&type9);
  auto cell_clip_param = model->addOperand(&type13);
  auto proj_clip_param = model->addOperand(&type13);
  auto time_major_param = model->addOperand(&type11);
  auto input_layer_norm_weights = model->addOperand(&type16);
  auto forget_layer_norm_weights = model->addOperand(&type14);
  auto cell_layer_norm_weights = model->addOperand(&type14);
  auto output_layer_norm_weights = model->addOperand(&type14);
  auto output = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t activation_param_init[] = {4};
  model->setOperandValue(activation_param, activation_param_init, sizeof(int32_t) * 1);
  static _Float16 cell_clip_param_init[] = {0.0f};
  model->setOperandValue(cell_clip_param, cell_clip_param_init, sizeof(_Float16) * 1);
  static _Float16 proj_clip_param_init[] = {0.0f};
  model->setOperandValue(proj_clip_param, proj_clip_param_init, sizeof(_Float16) * 1);
  static bool8 time_major_param_init[] = {true};
  model->setOperandValue(time_major_param, time_major_param_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_UNIDIRECTIONAL_SEQUENCE_LSTM, {input, input_to_input_weights, input_to_forget_weights, input_to_cell_weights, input_to_output_weights, recurrent_to_intput_weights, recurrent_to_forget_weights, recurrent_to_cell_weights, recurrent_to_output_weights, cell_to_input_weights, cell_to_forget_weights, cell_to_output_weights, input_gate_bias, forget_gate_bias, cell_gate_bias, output_gate_bias, projection_weights, projection_bias, output_state_in, cell_state_in, activation_param, cell_clip_param, proj_clip_param, time_major_param, input_layer_norm_weights, forget_layer_norm_weights, cell_layer_norm_weights, output_layer_norm_weights}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, input_to_input_weights, input_to_forget_weights, input_to_cell_weights, input_to_output_weights, recurrent_to_intput_weights, recurrent_to_forget_weights, recurrent_to_cell_weights, recurrent_to_output_weights, cell_to_input_weights, cell_to_forget_weights, cell_to_output_weights, input_gate_bias, forget_gate_bias, cell_gate_bias, output_gate_bias, projection_weights, projection_bias, output_state_in, cell_state_in, input_layer_norm_weights, forget_layer_norm_weights, cell_layer_norm_weights, output_layer_norm_weights},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

