// clang-format off
// Generated file (from: layer_norm_lstm.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 5});
  OperandType type1(Type::TENSOR_FLOAT32, {4, 5});
  OperandType type10(Type::TENSOR_FLOAT32, {2, 16});
  OperandType type2(Type::TENSOR_FLOAT32, {4, 3});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::TENSOR_FLOAT32, {3, 4});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 3});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 4});
  OperandType type8(Type::INT32, {});
  OperandType type9(Type::FLOAT32, {});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto input_to_input_weights = model->addOperand(&type1);
  auto input_to_forget_weights = model->addOperand(&type1);
  auto input_to_cell_weights = model->addOperand(&type1);
  auto input_to_output_weights = model->addOperand(&type1);
  auto recurrent_to_intput_weights = model->addOperand(&type2);
  auto recurrent_to_forget_weights = model->addOperand(&type2);
  auto recurrent_to_cell_weights = model->addOperand(&type2);
  auto recurrent_to_output_weights = model->addOperand(&type2);
  auto cell_to_input_weights = model->addOperand(&type3);
  auto cell_to_forget_weights = model->addOperand(&type3);
  auto cell_to_output_weights = model->addOperand(&type3);
  auto input_gate_bias = model->addOperand(&type3);
  auto forget_gate_bias = model->addOperand(&type3);
  auto cell_gate_bias = model->addOperand(&type3);
  auto output_gate_bias = model->addOperand(&type3);
  auto projection_weights = model->addOperand(&type4);
  auto projection_bias = model->addOperand(&type5);
  auto output_state_in = model->addOperand(&type6);
  auto cell_state_in = model->addOperand(&type7);
  auto activation_param = model->addOperand(&type8);
  auto cell_clip_param = model->addOperand(&type9);
  auto proj_clip_param = model->addOperand(&type9);
  auto input_layer_norm_weights = model->addOperand(&type3);
  auto forget_layer_norm_weights = model->addOperand(&type3);
  auto cell_layer_norm_weights = model->addOperand(&type3);
  auto output_layer_norm_weights = model->addOperand(&type3);
  auto scratch_buffer = model->addOperand(&type10);
  auto output_state_out = model->addOperand(&type6);
  auto cell_state_out = model->addOperand(&type7);
  auto output = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t activation_param_init[] = {4};
  model->setOperandValue(activation_param, activation_param_init, sizeof(int32_t) * 1);
  static float cell_clip_param_init[] = {0.0f};
  model->setOperandValue(cell_clip_param, cell_clip_param_init, sizeof(float) * 1);
  static float proj_clip_param_init[] = {0.0f};
  model->setOperandValue(proj_clip_param, proj_clip_param_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_LSTM, {input, input_to_input_weights, input_to_forget_weights, input_to_cell_weights, input_to_output_weights, recurrent_to_intput_weights, recurrent_to_forget_weights, recurrent_to_cell_weights, recurrent_to_output_weights, cell_to_input_weights, cell_to_forget_weights, cell_to_output_weights, input_gate_bias, forget_gate_bias, cell_gate_bias, output_gate_bias, projection_weights, projection_bias, output_state_in, cell_state_in, activation_param, cell_clip_param, proj_clip_param, input_layer_norm_weights, forget_layer_norm_weights, cell_layer_norm_weights, output_layer_norm_weights}, {scratch_buffer, output_state_out, cell_state_out, output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, input_to_input_weights, input_to_forget_weights, input_to_cell_weights, input_to_output_weights, recurrent_to_intput_weights, recurrent_to_forget_weights, recurrent_to_cell_weights, recurrent_to_output_weights, cell_to_input_weights, cell_to_forget_weights, cell_to_output_weights, input_gate_bias, forget_gate_bias, cell_gate_bias, output_gate_bias, projection_weights, projection_bias, output_state_in, cell_state_in, input_layer_norm_weights, forget_layer_norm_weights, cell_layer_norm_weights, output_layer_norm_weights},
    {scratch_buffer, output_state_out, cell_state_out, output});
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {0};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 5});
  OperandType type1(Type::TENSOR_FLOAT32, {4, 5});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {4, 3});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::TENSOR_FLOAT32, {3, 4});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 3});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 4});
  OperandType type8(Type::INT32, {});
  OperandType type9(Type::FLOAT32, {});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto input_to_input_weights = model->addOperand(&type1);
  auto input_to_forget_weights = model->addOperand(&type1);
  auto input_to_cell_weights = model->addOperand(&type1);
  auto input_to_output_weights = model->addOperand(&type1);
  auto recurrent_to_intput_weights = model->addOperand(&type2);
  auto recurrent_to_forget_weights = model->addOperand(&type2);
  auto recurrent_to_cell_weights = model->addOperand(&type2);
  auto recurrent_to_output_weights = model->addOperand(&type2);
  auto cell_to_input_weights = model->addOperand(&type3);
  auto cell_to_forget_weights = model->addOperand(&type3);
  auto cell_to_output_weights = model->addOperand(&type3);
  auto input_gate_bias = model->addOperand(&type3);
  auto forget_gate_bias = model->addOperand(&type3);
  auto cell_gate_bias = model->addOperand(&type3);
  auto output_gate_bias = model->addOperand(&type3);
  auto projection_weights = model->addOperand(&type4);
  auto projection_bias = model->addOperand(&type5);
  auto output_state_in = model->addOperand(&type6);
  auto cell_state_in = model->addOperand(&type7);
  auto activation_param = model->addOperand(&type8);
  auto cell_clip_param = model->addOperand(&type9);
  auto proj_clip_param = model->addOperand(&type9);
  auto input_layer_norm_weights = model->addOperand(&type3);
  auto forget_layer_norm_weights = model->addOperand(&type3);
  auto cell_layer_norm_weights = model->addOperand(&type3);
  auto output_layer_norm_weights = model->addOperand(&type3);
  auto scratch_buffer = model->addOperand(&type11);
  auto output_state_out = model->addOperand(&type11);
  auto cell_state_out = model->addOperand(&type11);
  auto output = model->addOperand(&type11);
  // Phase 2, operations
  static int32_t activation_param_init[] = {4};
  model->setOperandValue(activation_param, activation_param_init, sizeof(int32_t) * 1);
  static float cell_clip_param_init[] = {0.0f};
  model->setOperandValue(cell_clip_param, cell_clip_param_init, sizeof(float) * 1);
  static float proj_clip_param_init[] = {0.0f};
  model->setOperandValue(proj_clip_param, proj_clip_param_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_LSTM, {input, input_to_input_weights, input_to_forget_weights, input_to_cell_weights, input_to_output_weights, recurrent_to_intput_weights, recurrent_to_forget_weights, recurrent_to_cell_weights, recurrent_to_output_weights, cell_to_input_weights, cell_to_forget_weights, cell_to_output_weights, input_gate_bias, forget_gate_bias, cell_gate_bias, output_gate_bias, projection_weights, projection_bias, output_state_in, cell_state_in, activation_param, cell_clip_param, proj_clip_param, input_layer_norm_weights, forget_layer_norm_weights, cell_layer_norm_weights, output_layer_norm_weights}, {scratch_buffer, output_state_out, cell_state_out, output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, input_to_input_weights, input_to_forget_weights, input_to_cell_weights, input_to_output_weights, recurrent_to_intput_weights, recurrent_to_forget_weights, recurrent_to_cell_weights, recurrent_to_output_weights, cell_to_input_weights, cell_to_forget_weights, cell_to_output_weights, input_gate_bias, forget_gate_bias, cell_gate_bias, output_gate_bias, projection_weights, projection_bias, output_state_in, cell_state_in, input_layer_norm_weights, forget_layer_norm_weights, cell_layer_norm_weights, output_layer_norm_weights},
    {scratch_buffer, output_state_out, cell_state_out, output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {0};
  return ignore.find(i) != ignore.end();
}

void CreateModel_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 5});
  OperandType type1(Type::TENSOR_FLOAT32, {4, 5});
  OperandType type10(Type::TENSOR_FLOAT32, {2, 16});
  OperandType type2(Type::TENSOR_FLOAT32, {4, 3});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::TENSOR_FLOAT32, {3, 4});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 3});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 4});
  OperandType type8(Type::INT32, {});
  OperandType type9(Type::FLOAT32, {});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto input_to_input_weights = model->addOperand(&type1);
  auto input_to_forget_weights = model->addOperand(&type1);
  auto input_to_cell_weights = model->addOperand(&type1);
  auto input_to_output_weights = model->addOperand(&type1);
  auto recurrent_to_intput_weights = model->addOperand(&type2);
  auto recurrent_to_forget_weights = model->addOperand(&type2);
  auto recurrent_to_cell_weights = model->addOperand(&type2);
  auto recurrent_to_output_weights = model->addOperand(&type2);
  auto cell_to_input_weights = model->addOperand(&type3);
  auto cell_to_forget_weights = model->addOperand(&type3);
  auto cell_to_output_weights = model->addOperand(&type3);
  auto input_gate_bias = model->addOperand(&type3);
  auto forget_gate_bias = model->addOperand(&type3);
  auto cell_gate_bias = model->addOperand(&type3);
  auto output_gate_bias = model->addOperand(&type3);
  auto projection_weights = model->addOperand(&type4);
  auto projection_bias = model->addOperand(&type5);
  auto output_state_in = model->addOperand(&type6);
  auto cell_state_in = model->addOperand(&type7);
  auto activation_param = model->addOperand(&type8);
  auto cell_clip_param = model->addOperand(&type9);
  auto proj_clip_param = model->addOperand(&type9);
  auto input_layer_norm_weights = model->addOperand(&type3);
  auto forget_layer_norm_weights = model->addOperand(&type3);
  auto cell_layer_norm_weights = model->addOperand(&type3);
  auto output_layer_norm_weights = model->addOperand(&type3);
  auto scratch_buffer = model->addOperand(&type10);
  auto output_state_out = model->addOperand(&type6);
  auto cell_state_out = model->addOperand(&type7);
  auto output = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t activation_param_init[] = {4};
  model->setOperandValue(activation_param, activation_param_init, sizeof(int32_t) * 1);
  static float cell_clip_param_init[] = {0.0f};
  model->setOperandValue(cell_clip_param, cell_clip_param_init, sizeof(float) * 1);
  static float proj_clip_param_init[] = {0.0f};
  model->setOperandValue(proj_clip_param, proj_clip_param_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_LSTM, {input, input_to_input_weights, input_to_forget_weights, input_to_cell_weights, input_to_output_weights, recurrent_to_intput_weights, recurrent_to_forget_weights, recurrent_to_cell_weights, recurrent_to_output_weights, cell_to_input_weights, cell_to_forget_weights, cell_to_output_weights, input_gate_bias, forget_gate_bias, cell_gate_bias, output_gate_bias, projection_weights, projection_bias, output_state_in, cell_state_in, activation_param, cell_clip_param, proj_clip_param, input_layer_norm_weights, forget_layer_norm_weights, cell_layer_norm_weights, output_layer_norm_weights}, {scratch_buffer, output_state_out, cell_state_out, output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, input_to_input_weights, input_to_forget_weights, input_to_cell_weights, input_to_output_weights, recurrent_to_intput_weights, recurrent_to_forget_weights, recurrent_to_cell_weights, recurrent_to_output_weights, cell_to_input_weights, cell_to_forget_weights, cell_to_output_weights, input_gate_bias, forget_gate_bias, cell_gate_bias, output_gate_bias, projection_weights, projection_bias, output_state_in, cell_state_in, input_layer_norm_weights, forget_layer_norm_weights, cell_layer_norm_weights, output_layer_norm_weights},
    {scratch_buffer, output_state_out, cell_state_out, output});
  assert(model->isValid());
}

inline bool is_ignored_2(int i) {
  static std::set<int> ignore = {0};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 5});
  OperandType type1(Type::TENSOR_FLOAT32, {4, 5});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {4, 3});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::TENSOR_FLOAT32, {3, 4});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 3});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 4});
  OperandType type8(Type::INT32, {});
  OperandType type9(Type::FLOAT32, {});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto input_to_input_weights = model->addOperand(&type1);
  auto input_to_forget_weights = model->addOperand(&type1);
  auto input_to_cell_weights = model->addOperand(&type1);
  auto input_to_output_weights = model->addOperand(&type1);
  auto recurrent_to_intput_weights = model->addOperand(&type2);
  auto recurrent_to_forget_weights = model->addOperand(&type2);
  auto recurrent_to_cell_weights = model->addOperand(&type2);
  auto recurrent_to_output_weights = model->addOperand(&type2);
  auto cell_to_input_weights = model->addOperand(&type3);
  auto cell_to_forget_weights = model->addOperand(&type3);
  auto cell_to_output_weights = model->addOperand(&type3);
  auto input_gate_bias = model->addOperand(&type3);
  auto forget_gate_bias = model->addOperand(&type3);
  auto cell_gate_bias = model->addOperand(&type3);
  auto output_gate_bias = model->addOperand(&type3);
  auto projection_weights = model->addOperand(&type4);
  auto projection_bias = model->addOperand(&type5);
  auto output_state_in = model->addOperand(&type6);
  auto cell_state_in = model->addOperand(&type7);
  auto activation_param = model->addOperand(&type8);
  auto cell_clip_param = model->addOperand(&type9);
  auto proj_clip_param = model->addOperand(&type9);
  auto input_layer_norm_weights = model->addOperand(&type3);
  auto forget_layer_norm_weights = model->addOperand(&type3);
  auto cell_layer_norm_weights = model->addOperand(&type3);
  auto output_layer_norm_weights = model->addOperand(&type3);
  auto scratch_buffer = model->addOperand(&type11);
  auto output_state_out = model->addOperand(&type11);
  auto cell_state_out = model->addOperand(&type11);
  auto output = model->addOperand(&type11);
  // Phase 2, operations
  static int32_t activation_param_init[] = {4};
  model->setOperandValue(activation_param, activation_param_init, sizeof(int32_t) * 1);
  static float cell_clip_param_init[] = {0.0f};
  model->setOperandValue(cell_clip_param, cell_clip_param_init, sizeof(float) * 1);
  static float proj_clip_param_init[] = {0.0f};
  model->setOperandValue(proj_clip_param, proj_clip_param_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_LSTM, {input, input_to_input_weights, input_to_forget_weights, input_to_cell_weights, input_to_output_weights, recurrent_to_intput_weights, recurrent_to_forget_weights, recurrent_to_cell_weights, recurrent_to_output_weights, cell_to_input_weights, cell_to_forget_weights, cell_to_output_weights, input_gate_bias, forget_gate_bias, cell_gate_bias, output_gate_bias, projection_weights, projection_bias, output_state_in, cell_state_in, activation_param, cell_clip_param, proj_clip_param, input_layer_norm_weights, forget_layer_norm_weights, cell_layer_norm_weights, output_layer_norm_weights}, {scratch_buffer, output_state_out, cell_state_out, output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, input_to_input_weights, input_to_forget_weights, input_to_cell_weights, input_to_output_weights, recurrent_to_intput_weights, recurrent_to_forget_weights, recurrent_to_cell_weights, recurrent_to_output_weights, cell_to_input_weights, cell_to_forget_weights, cell_to_output_weights, input_gate_bias, forget_gate_bias, cell_gate_bias, output_gate_bias, projection_weights, projection_bias, output_state_in, cell_state_in, input_layer_norm_weights, forget_layer_norm_weights, cell_layer_norm_weights, output_layer_norm_weights},
    {scratch_buffer, output_state_out, cell_state_out, output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_2(int i) {
  static std::set<int> ignore = {0};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 5});
  OperandType type1(Type::TENSOR_FLOAT32, {4, 5});
  OperandType type10(Type::TENSOR_FLOAT32, {2, 16});
  OperandType type2(Type::TENSOR_FLOAT32, {4, 3});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::TENSOR_FLOAT32, {3, 4});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 3});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 4});
  OperandType type8(Type::INT32, {});
  OperandType type9(Type::FLOAT32, {});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto input_to_input_weights = model->addOperand(&type1);
  auto input_to_forget_weights = model->addOperand(&type1);
  auto input_to_cell_weights = model->addOperand(&type1);
  auto input_to_output_weights = model->addOperand(&type1);
  auto recurrent_to_intput_weights = model->addOperand(&type2);
  auto recurrent_to_forget_weights = model->addOperand(&type2);
  auto recurrent_to_cell_weights = model->addOperand(&type2);
  auto recurrent_to_output_weights = model->addOperand(&type2);
  auto cell_to_input_weights = model->addOperand(&type3);
  auto cell_to_forget_weights = model->addOperand(&type3);
  auto cell_to_output_weights = model->addOperand(&type3);
  auto input_gate_bias = model->addOperand(&type3);
  auto forget_gate_bias = model->addOperand(&type3);
  auto cell_gate_bias = model->addOperand(&type3);
  auto output_gate_bias = model->addOperand(&type3);
  auto projection_weights = model->addOperand(&type4);
  auto projection_bias = model->addOperand(&type5);
  auto output_state_in = model->addOperand(&type6);
  auto cell_state_in = model->addOperand(&type7);
  auto activation_param = model->addOperand(&type8);
  auto cell_clip_param = model->addOperand(&type9);
  auto proj_clip_param = model->addOperand(&type9);
  auto input_layer_norm_weights = model->addOperand(&type3);
  auto forget_layer_norm_weights = model->addOperand(&type3);
  auto cell_layer_norm_weights = model->addOperand(&type3);
  auto output_layer_norm_weights = model->addOperand(&type3);
  auto scratch_buffer = model->addOperand(&type10);
  auto output_state_out = model->addOperand(&type6);
  auto cell_state_out = model->addOperand(&type7);
  auto output = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t activation_param_init[] = {4};
  model->setOperandValue(activation_param, activation_param_init, sizeof(int32_t) * 1);
  static float cell_clip_param_init[] = {0.0f};
  model->setOperandValue(cell_clip_param, cell_clip_param_init, sizeof(float) * 1);
  static float proj_clip_param_init[] = {0.0f};
  model->setOperandValue(proj_clip_param, proj_clip_param_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_LSTM, {input, input_to_input_weights, input_to_forget_weights, input_to_cell_weights, input_to_output_weights, recurrent_to_intput_weights, recurrent_to_forget_weights, recurrent_to_cell_weights, recurrent_to_output_weights, cell_to_input_weights, cell_to_forget_weights, cell_to_output_weights, input_gate_bias, forget_gate_bias, cell_gate_bias, output_gate_bias, projection_weights, projection_bias, output_state_in, cell_state_in, activation_param, cell_clip_param, proj_clip_param, input_layer_norm_weights, forget_layer_norm_weights, cell_layer_norm_weights, output_layer_norm_weights}, {scratch_buffer, output_state_out, cell_state_out, output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, input_to_input_weights, input_to_forget_weights, input_to_cell_weights, input_to_output_weights, recurrent_to_intput_weights, recurrent_to_forget_weights, recurrent_to_cell_weights, recurrent_to_output_weights, cell_to_input_weights, cell_to_forget_weights, cell_to_output_weights, input_gate_bias, forget_gate_bias, cell_gate_bias, output_gate_bias, projection_weights, projection_bias, output_state_in, cell_state_in, input_layer_norm_weights, forget_layer_norm_weights, cell_layer_norm_weights, output_layer_norm_weights},
    {scratch_buffer, output_state_out, cell_state_out, output});
  assert(model->isValid());
}

inline bool is_ignored_3(int i) {
  static std::set<int> ignore = {0};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_3(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 5});
  OperandType type1(Type::TENSOR_FLOAT32, {4, 5});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {4, 3});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::TENSOR_FLOAT32, {3, 4});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 3});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 4});
  OperandType type8(Type::INT32, {});
  OperandType type9(Type::FLOAT32, {});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto input_to_input_weights = model->addOperand(&type1);
  auto input_to_forget_weights = model->addOperand(&type1);
  auto input_to_cell_weights = model->addOperand(&type1);
  auto input_to_output_weights = model->addOperand(&type1);
  auto recurrent_to_intput_weights = model->addOperand(&type2);
  auto recurrent_to_forget_weights = model->addOperand(&type2);
  auto recurrent_to_cell_weights = model->addOperand(&type2);
  auto recurrent_to_output_weights = model->addOperand(&type2);
  auto cell_to_input_weights = model->addOperand(&type3);
  auto cell_to_forget_weights = model->addOperand(&type3);
  auto cell_to_output_weights = model->addOperand(&type3);
  auto input_gate_bias = model->addOperand(&type3);
  auto forget_gate_bias = model->addOperand(&type3);
  auto cell_gate_bias = model->addOperand(&type3);
  auto output_gate_bias = model->addOperand(&type3);
  auto projection_weights = model->addOperand(&type4);
  auto projection_bias = model->addOperand(&type5);
  auto output_state_in = model->addOperand(&type6);
  auto cell_state_in = model->addOperand(&type7);
  auto activation_param = model->addOperand(&type8);
  auto cell_clip_param = model->addOperand(&type9);
  auto proj_clip_param = model->addOperand(&type9);
  auto input_layer_norm_weights = model->addOperand(&type3);
  auto forget_layer_norm_weights = model->addOperand(&type3);
  auto cell_layer_norm_weights = model->addOperand(&type3);
  auto output_layer_norm_weights = model->addOperand(&type3);
  auto scratch_buffer = model->addOperand(&type11);
  auto output_state_out = model->addOperand(&type11);
  auto cell_state_out = model->addOperand(&type11);
  auto output = model->addOperand(&type11);
  // Phase 2, operations
  static int32_t activation_param_init[] = {4};
  model->setOperandValue(activation_param, activation_param_init, sizeof(int32_t) * 1);
  static float cell_clip_param_init[] = {0.0f};
  model->setOperandValue(cell_clip_param, cell_clip_param_init, sizeof(float) * 1);
  static float proj_clip_param_init[] = {0.0f};
  model->setOperandValue(proj_clip_param, proj_clip_param_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_LSTM, {input, input_to_input_weights, input_to_forget_weights, input_to_cell_weights, input_to_output_weights, recurrent_to_intput_weights, recurrent_to_forget_weights, recurrent_to_cell_weights, recurrent_to_output_weights, cell_to_input_weights, cell_to_forget_weights, cell_to_output_weights, input_gate_bias, forget_gate_bias, cell_gate_bias, output_gate_bias, projection_weights, projection_bias, output_state_in, cell_state_in, activation_param, cell_clip_param, proj_clip_param, input_layer_norm_weights, forget_layer_norm_weights, cell_layer_norm_weights, output_layer_norm_weights}, {scratch_buffer, output_state_out, cell_state_out, output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, input_to_input_weights, input_to_forget_weights, input_to_cell_weights, input_to_output_weights, recurrent_to_intput_weights, recurrent_to_forget_weights, recurrent_to_cell_weights, recurrent_to_output_weights, cell_to_input_weights, cell_to_forget_weights, cell_to_output_weights, input_gate_bias, forget_gate_bias, cell_gate_bias, output_gate_bias, projection_weights, projection_bias, output_state_in, cell_state_in, input_layer_norm_weights, forget_layer_norm_weights, cell_layer_norm_weights, output_layer_norm_weights},
    {scratch_buffer, output_state_out, cell_state_out, output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_3(int i) {
  static std::set<int> ignore = {0};
  return ignore.find(i) != ignore.end();
}

void CreateModel_4(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 5});
  OperandType type1(Type::TENSOR_FLOAT32, {4, 5});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type12(Type::TENSOR_FLOAT32, {2, 12});
  OperandType type2(Type::TENSOR_FLOAT32, {4, 3});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::TENSOR_FLOAT32, {3, 4});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 3});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 4});
  OperandType type8(Type::INT32, {});
  OperandType type9(Type::FLOAT32, {});
  // Phase 1, operands
  auto input1 = model->addOperand(&type0);
  auto input_to_input_weights1 = model->addOperand(&type11);
  auto input_to_forget_weights1 = model->addOperand(&type1);
  auto input_to_cell_weights1 = model->addOperand(&type1);
  auto input_to_output_weights1 = model->addOperand(&type1);
  auto recurrent_to_intput_weights1 = model->addOperand(&type11);
  auto recurrent_to_forget_weights1 = model->addOperand(&type2);
  auto recurrent_to_cell_weights1 = model->addOperand(&type2);
  auto recurrent_to_output_weights1 = model->addOperand(&type2);
  auto cell_to_input_weights1 = model->addOperand(&type5);
  auto cell_to_forget_weights1 = model->addOperand(&type3);
  auto cell_to_output_weights1 = model->addOperand(&type3);
  auto input_gate_bias1 = model->addOperand(&type5);
  auto forget_gate_bias1 = model->addOperand(&type3);
  auto cell_gate_bias1 = model->addOperand(&type3);
  auto output_gate_bias1 = model->addOperand(&type3);
  auto projection_weights1 = model->addOperand(&type4);
  auto projection_bias1 = model->addOperand(&type5);
  auto output_state_in1 = model->addOperand(&type6);
  auto cell_state_in1 = model->addOperand(&type7);
  auto activation_param1 = model->addOperand(&type8);
  auto cell_clip_param1 = model->addOperand(&type9);
  auto proj_clip_param1 = model->addOperand(&type9);
  auto input_layer_norm_weights1 = model->addOperand(&type5);
  auto forget_layer_norm_weights1 = model->addOperand(&type3);
  auto cell_layer_norm_weights1 = model->addOperand(&type3);
  auto output_layer_norm_weights1 = model->addOperand(&type3);
  auto scratch_buffer1 = model->addOperand(&type12);
  auto output_state_out1 = model->addOperand(&type6);
  auto cell_state_out1 = model->addOperand(&type7);
  auto output1 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t activation_param1_init[] = {4};
  model->setOperandValue(activation_param1, activation_param1_init, sizeof(int32_t) * 1);
  static float cell_clip_param1_init[] = {0.0f};
  model->setOperandValue(cell_clip_param1, cell_clip_param1_init, sizeof(float) * 1);
  static float proj_clip_param1_init[] = {0.0f};
  model->setOperandValue(proj_clip_param1, proj_clip_param1_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_LSTM, {input1, input_to_input_weights1, input_to_forget_weights1, input_to_cell_weights1, input_to_output_weights1, recurrent_to_intput_weights1, recurrent_to_forget_weights1, recurrent_to_cell_weights1, recurrent_to_output_weights1, cell_to_input_weights1, cell_to_forget_weights1, cell_to_output_weights1, input_gate_bias1, forget_gate_bias1, cell_gate_bias1, output_gate_bias1, projection_weights1, projection_bias1, output_state_in1, cell_state_in1, activation_param1, cell_clip_param1, proj_clip_param1, input_layer_norm_weights1, forget_layer_norm_weights1, cell_layer_norm_weights1, output_layer_norm_weights1}, {scratch_buffer1, output_state_out1, cell_state_out1, output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input1, input_to_input_weights1, input_to_forget_weights1, input_to_cell_weights1, input_to_output_weights1, recurrent_to_intput_weights1, recurrent_to_forget_weights1, recurrent_to_cell_weights1, recurrent_to_output_weights1, cell_to_input_weights1, cell_to_forget_weights1, cell_to_output_weights1, input_gate_bias1, forget_gate_bias1, cell_gate_bias1, output_gate_bias1, projection_weights1, projection_bias1, output_state_in1, cell_state_in1, input_layer_norm_weights1, forget_layer_norm_weights1, cell_layer_norm_weights1, output_layer_norm_weights1},
    {scratch_buffer1, output_state_out1, cell_state_out1, output1});
  assert(model->isValid());
}

inline bool is_ignored_4(int i) {
  static std::set<int> ignore = {0, 1};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_4(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 5});
  OperandType type1(Type::TENSOR_FLOAT32, {4, 5});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {4, 3});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::TENSOR_FLOAT32, {3, 4});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 3});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 4});
  OperandType type8(Type::INT32, {});
  OperandType type9(Type::FLOAT32, {});
  // Phase 1, operands
  auto input1 = model->addOperand(&type0);
  auto input_to_input_weights1 = model->addOperand(&type11);
  auto input_to_forget_weights1 = model->addOperand(&type1);
  auto input_to_cell_weights1 = model->addOperand(&type1);
  auto input_to_output_weights1 = model->addOperand(&type1);
  auto recurrent_to_intput_weights1 = model->addOperand(&type11);
  auto recurrent_to_forget_weights1 = model->addOperand(&type2);
  auto recurrent_to_cell_weights1 = model->addOperand(&type2);
  auto recurrent_to_output_weights1 = model->addOperand(&type2);
  auto cell_to_input_weights1 = model->addOperand(&type5);
  auto cell_to_forget_weights1 = model->addOperand(&type3);
  auto cell_to_output_weights1 = model->addOperand(&type3);
  auto input_gate_bias1 = model->addOperand(&type5);
  auto forget_gate_bias1 = model->addOperand(&type3);
  auto cell_gate_bias1 = model->addOperand(&type3);
  auto output_gate_bias1 = model->addOperand(&type3);
  auto projection_weights1 = model->addOperand(&type4);
  auto projection_bias1 = model->addOperand(&type5);
  auto output_state_in1 = model->addOperand(&type6);
  auto cell_state_in1 = model->addOperand(&type7);
  auto activation_param1 = model->addOperand(&type8);
  auto cell_clip_param1 = model->addOperand(&type9);
  auto proj_clip_param1 = model->addOperand(&type9);
  auto input_layer_norm_weights1 = model->addOperand(&type5);
  auto forget_layer_norm_weights1 = model->addOperand(&type3);
  auto cell_layer_norm_weights1 = model->addOperand(&type3);
  auto output_layer_norm_weights1 = model->addOperand(&type3);
  auto scratch_buffer1 = model->addOperand(&type11);
  auto output_state_out1 = model->addOperand(&type11);
  auto cell_state_out1 = model->addOperand(&type11);
  auto output1 = model->addOperand(&type11);
  // Phase 2, operations
  static int32_t activation_param1_init[] = {4};
  model->setOperandValue(activation_param1, activation_param1_init, sizeof(int32_t) * 1);
  static float cell_clip_param1_init[] = {0.0f};
  model->setOperandValue(cell_clip_param1, cell_clip_param1_init, sizeof(float) * 1);
  static float proj_clip_param1_init[] = {0.0f};
  model->setOperandValue(proj_clip_param1, proj_clip_param1_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_LSTM, {input1, input_to_input_weights1, input_to_forget_weights1, input_to_cell_weights1, input_to_output_weights1, recurrent_to_intput_weights1, recurrent_to_forget_weights1, recurrent_to_cell_weights1, recurrent_to_output_weights1, cell_to_input_weights1, cell_to_forget_weights1, cell_to_output_weights1, input_gate_bias1, forget_gate_bias1, cell_gate_bias1, output_gate_bias1, projection_weights1, projection_bias1, output_state_in1, cell_state_in1, activation_param1, cell_clip_param1, proj_clip_param1, input_layer_norm_weights1, forget_layer_norm_weights1, cell_layer_norm_weights1, output_layer_norm_weights1}, {scratch_buffer1, output_state_out1, cell_state_out1, output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input1, input_to_input_weights1, input_to_forget_weights1, input_to_cell_weights1, input_to_output_weights1, recurrent_to_intput_weights1, recurrent_to_forget_weights1, recurrent_to_cell_weights1, recurrent_to_output_weights1, cell_to_input_weights1, cell_to_forget_weights1, cell_to_output_weights1, input_gate_bias1, forget_gate_bias1, cell_gate_bias1, output_gate_bias1, projection_weights1, projection_bias1, output_state_in1, cell_state_in1, input_layer_norm_weights1, forget_layer_norm_weights1, cell_layer_norm_weights1, output_layer_norm_weights1},
    {scratch_buffer1, output_state_out1, cell_state_out1, output1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_4(int i) {
  static std::set<int> ignore = {0, 1};
  return ignore.find(i) != ignore.end();
}

void CreateModel_5(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 5});
  OperandType type1(Type::TENSOR_FLOAT32, {4, 5});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type12(Type::TENSOR_FLOAT32, {2, 12});
  OperandType type2(Type::TENSOR_FLOAT32, {4, 3});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::TENSOR_FLOAT32, {3, 4});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 3});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 4});
  OperandType type8(Type::INT32, {});
  OperandType type9(Type::FLOAT32, {});
  // Phase 1, operands
  auto input1 = model->addOperand(&type0);
  auto input_to_input_weights1 = model->addOperand(&type11);
  auto input_to_forget_weights1 = model->addOperand(&type1);
  auto input_to_cell_weights1 = model->addOperand(&type1);
  auto input_to_output_weights1 = model->addOperand(&type1);
  auto recurrent_to_intput_weights1 = model->addOperand(&type11);
  auto recurrent_to_forget_weights1 = model->addOperand(&type2);
  auto recurrent_to_cell_weights1 = model->addOperand(&type2);
  auto recurrent_to_output_weights1 = model->addOperand(&type2);
  auto cell_to_input_weights1 = model->addOperand(&type5);
  auto cell_to_forget_weights1 = model->addOperand(&type3);
  auto cell_to_output_weights1 = model->addOperand(&type3);
  auto input_gate_bias1 = model->addOperand(&type5);
  auto forget_gate_bias1 = model->addOperand(&type3);
  auto cell_gate_bias1 = model->addOperand(&type3);
  auto output_gate_bias1 = model->addOperand(&type3);
  auto projection_weights1 = model->addOperand(&type4);
  auto projection_bias1 = model->addOperand(&type5);
  auto output_state_in1 = model->addOperand(&type6);
  auto cell_state_in1 = model->addOperand(&type7);
  auto activation_param1 = model->addOperand(&type8);
  auto cell_clip_param1 = model->addOperand(&type9);
  auto proj_clip_param1 = model->addOperand(&type9);
  auto input_layer_norm_weights1 = model->addOperand(&type5);
  auto forget_layer_norm_weights1 = model->addOperand(&type3);
  auto cell_layer_norm_weights1 = model->addOperand(&type3);
  auto output_layer_norm_weights1 = model->addOperand(&type3);
  auto scratch_buffer1 = model->addOperand(&type12);
  auto output_state_out1 = model->addOperand(&type6);
  auto cell_state_out1 = model->addOperand(&type7);
  auto output1 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t activation_param1_init[] = {4};
  model->setOperandValue(activation_param1, activation_param1_init, sizeof(int32_t) * 1);
  static float cell_clip_param1_init[] = {0.0f};
  model->setOperandValue(cell_clip_param1, cell_clip_param1_init, sizeof(float) * 1);
  static float proj_clip_param1_init[] = {0.0f};
  model->setOperandValue(proj_clip_param1, proj_clip_param1_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_LSTM, {input1, input_to_input_weights1, input_to_forget_weights1, input_to_cell_weights1, input_to_output_weights1, recurrent_to_intput_weights1, recurrent_to_forget_weights1, recurrent_to_cell_weights1, recurrent_to_output_weights1, cell_to_input_weights1, cell_to_forget_weights1, cell_to_output_weights1, input_gate_bias1, forget_gate_bias1, cell_gate_bias1, output_gate_bias1, projection_weights1, projection_bias1, output_state_in1, cell_state_in1, activation_param1, cell_clip_param1, proj_clip_param1, input_layer_norm_weights1, forget_layer_norm_weights1, cell_layer_norm_weights1, output_layer_norm_weights1}, {scratch_buffer1, output_state_out1, cell_state_out1, output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input1, input_to_input_weights1, input_to_forget_weights1, input_to_cell_weights1, input_to_output_weights1, recurrent_to_intput_weights1, recurrent_to_forget_weights1, recurrent_to_cell_weights1, recurrent_to_output_weights1, cell_to_input_weights1, cell_to_forget_weights1, cell_to_output_weights1, input_gate_bias1, forget_gate_bias1, cell_gate_bias1, output_gate_bias1, projection_weights1, projection_bias1, output_state_in1, cell_state_in1, input_layer_norm_weights1, forget_layer_norm_weights1, cell_layer_norm_weights1, output_layer_norm_weights1},
    {scratch_buffer1, output_state_out1, cell_state_out1, output1});
  assert(model->isValid());
}

inline bool is_ignored_5(int i) {
  static std::set<int> ignore = {0, 1};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_5(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 5});
  OperandType type1(Type::TENSOR_FLOAT32, {4, 5});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {4, 3});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::TENSOR_FLOAT32, {3, 4});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 3});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 4});
  OperandType type8(Type::INT32, {});
  OperandType type9(Type::FLOAT32, {});
  // Phase 1, operands
  auto input1 = model->addOperand(&type0);
  auto input_to_input_weights1 = model->addOperand(&type11);
  auto input_to_forget_weights1 = model->addOperand(&type1);
  auto input_to_cell_weights1 = model->addOperand(&type1);
  auto input_to_output_weights1 = model->addOperand(&type1);
  auto recurrent_to_intput_weights1 = model->addOperand(&type11);
  auto recurrent_to_forget_weights1 = model->addOperand(&type2);
  auto recurrent_to_cell_weights1 = model->addOperand(&type2);
  auto recurrent_to_output_weights1 = model->addOperand(&type2);
  auto cell_to_input_weights1 = model->addOperand(&type5);
  auto cell_to_forget_weights1 = model->addOperand(&type3);
  auto cell_to_output_weights1 = model->addOperand(&type3);
  auto input_gate_bias1 = model->addOperand(&type5);
  auto forget_gate_bias1 = model->addOperand(&type3);
  auto cell_gate_bias1 = model->addOperand(&type3);
  auto output_gate_bias1 = model->addOperand(&type3);
  auto projection_weights1 = model->addOperand(&type4);
  auto projection_bias1 = model->addOperand(&type5);
  auto output_state_in1 = model->addOperand(&type6);
  auto cell_state_in1 = model->addOperand(&type7);
  auto activation_param1 = model->addOperand(&type8);
  auto cell_clip_param1 = model->addOperand(&type9);
  auto proj_clip_param1 = model->addOperand(&type9);
  auto input_layer_norm_weights1 = model->addOperand(&type5);
  auto forget_layer_norm_weights1 = model->addOperand(&type3);
  auto cell_layer_norm_weights1 = model->addOperand(&type3);
  auto output_layer_norm_weights1 = model->addOperand(&type3);
  auto scratch_buffer1 = model->addOperand(&type11);
  auto output_state_out1 = model->addOperand(&type11);
  auto cell_state_out1 = model->addOperand(&type11);
  auto output1 = model->addOperand(&type11);
  // Phase 2, operations
  static int32_t activation_param1_init[] = {4};
  model->setOperandValue(activation_param1, activation_param1_init, sizeof(int32_t) * 1);
  static float cell_clip_param1_init[] = {0.0f};
  model->setOperandValue(cell_clip_param1, cell_clip_param1_init, sizeof(float) * 1);
  static float proj_clip_param1_init[] = {0.0f};
  model->setOperandValue(proj_clip_param1, proj_clip_param1_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_LSTM, {input1, input_to_input_weights1, input_to_forget_weights1, input_to_cell_weights1, input_to_output_weights1, recurrent_to_intput_weights1, recurrent_to_forget_weights1, recurrent_to_cell_weights1, recurrent_to_output_weights1, cell_to_input_weights1, cell_to_forget_weights1, cell_to_output_weights1, input_gate_bias1, forget_gate_bias1, cell_gate_bias1, output_gate_bias1, projection_weights1, projection_bias1, output_state_in1, cell_state_in1, activation_param1, cell_clip_param1, proj_clip_param1, input_layer_norm_weights1, forget_layer_norm_weights1, cell_layer_norm_weights1, output_layer_norm_weights1}, {scratch_buffer1, output_state_out1, cell_state_out1, output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input1, input_to_input_weights1, input_to_forget_weights1, input_to_cell_weights1, input_to_output_weights1, recurrent_to_intput_weights1, recurrent_to_forget_weights1, recurrent_to_cell_weights1, recurrent_to_output_weights1, cell_to_input_weights1, cell_to_forget_weights1, cell_to_output_weights1, input_gate_bias1, forget_gate_bias1, cell_gate_bias1, output_gate_bias1, projection_weights1, projection_bias1, output_state_in1, cell_state_in1, input_layer_norm_weights1, forget_layer_norm_weights1, cell_layer_norm_weights1, output_layer_norm_weights1},
    {scratch_buffer1, output_state_out1, cell_state_out1, output1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_5(int i) {
  static std::set<int> ignore = {0, 1};
  return ignore.find(i) != ignore.end();
}

void CreateModel_6(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 5});
  OperandType type1(Type::TENSOR_FLOAT32, {4, 5});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type12(Type::TENSOR_FLOAT32, {2, 12});
  OperandType type2(Type::TENSOR_FLOAT32, {4, 3});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::TENSOR_FLOAT32, {3, 4});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 3});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 4});
  OperandType type8(Type::INT32, {});
  OperandType type9(Type::FLOAT32, {});
  // Phase 1, operands
  auto input1 = model->addOperand(&type0);
  auto input_to_input_weights1 = model->addOperand(&type11);
  auto input_to_forget_weights1 = model->addOperand(&type1);
  auto input_to_cell_weights1 = model->addOperand(&type1);
  auto input_to_output_weights1 = model->addOperand(&type1);
  auto recurrent_to_intput_weights1 = model->addOperand(&type11);
  auto recurrent_to_forget_weights1 = model->addOperand(&type2);
  auto recurrent_to_cell_weights1 = model->addOperand(&type2);
  auto recurrent_to_output_weights1 = model->addOperand(&type2);
  auto cell_to_input_weights1 = model->addOperand(&type5);
  auto cell_to_forget_weights1 = model->addOperand(&type3);
  auto cell_to_output_weights1 = model->addOperand(&type3);
  auto input_gate_bias1 = model->addOperand(&type5);
  auto forget_gate_bias1 = model->addOperand(&type3);
  auto cell_gate_bias1 = model->addOperand(&type3);
  auto output_gate_bias1 = model->addOperand(&type3);
  auto projection_weights1 = model->addOperand(&type4);
  auto projection_bias1 = model->addOperand(&type5);
  auto output_state_in1 = model->addOperand(&type6);
  auto cell_state_in1 = model->addOperand(&type7);
  auto activation_param1 = model->addOperand(&type8);
  auto cell_clip_param1 = model->addOperand(&type9);
  auto proj_clip_param1 = model->addOperand(&type9);
  auto input_layer_norm_weights1 = model->addOperand(&type5);
  auto forget_layer_norm_weights1 = model->addOperand(&type3);
  auto cell_layer_norm_weights1 = model->addOperand(&type3);
  auto output_layer_norm_weights1 = model->addOperand(&type3);
  auto scratch_buffer1 = model->addOperand(&type12);
  auto output_state_out1 = model->addOperand(&type6);
  auto cell_state_out1 = model->addOperand(&type7);
  auto output1 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t activation_param1_init[] = {4};
  model->setOperandValue(activation_param1, activation_param1_init, sizeof(int32_t) * 1);
  static float cell_clip_param1_init[] = {0.0f};
  model->setOperandValue(cell_clip_param1, cell_clip_param1_init, sizeof(float) * 1);
  static float proj_clip_param1_init[] = {0.0f};
  model->setOperandValue(proj_clip_param1, proj_clip_param1_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_LSTM, {input1, input_to_input_weights1, input_to_forget_weights1, input_to_cell_weights1, input_to_output_weights1, recurrent_to_intput_weights1, recurrent_to_forget_weights1, recurrent_to_cell_weights1, recurrent_to_output_weights1, cell_to_input_weights1, cell_to_forget_weights1, cell_to_output_weights1, input_gate_bias1, forget_gate_bias1, cell_gate_bias1, output_gate_bias1, projection_weights1, projection_bias1, output_state_in1, cell_state_in1, activation_param1, cell_clip_param1, proj_clip_param1, input_layer_norm_weights1, forget_layer_norm_weights1, cell_layer_norm_weights1, output_layer_norm_weights1}, {scratch_buffer1, output_state_out1, cell_state_out1, output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input1, input_to_input_weights1, input_to_forget_weights1, input_to_cell_weights1, input_to_output_weights1, recurrent_to_intput_weights1, recurrent_to_forget_weights1, recurrent_to_cell_weights1, recurrent_to_output_weights1, cell_to_input_weights1, cell_to_forget_weights1, cell_to_output_weights1, input_gate_bias1, forget_gate_bias1, cell_gate_bias1, output_gate_bias1, projection_weights1, projection_bias1, output_state_in1, cell_state_in1, input_layer_norm_weights1, forget_layer_norm_weights1, cell_layer_norm_weights1, output_layer_norm_weights1},
    {scratch_buffer1, output_state_out1, cell_state_out1, output1});
  assert(model->isValid());
}

inline bool is_ignored_6(int i) {
  static std::set<int> ignore = {0, 1};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_6(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 5});
  OperandType type1(Type::TENSOR_FLOAT32, {4, 5});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {4, 3});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::TENSOR_FLOAT32, {3, 4});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 3});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 4});
  OperandType type8(Type::INT32, {});
  OperandType type9(Type::FLOAT32, {});
  // Phase 1, operands
  auto input1 = model->addOperand(&type0);
  auto input_to_input_weights1 = model->addOperand(&type11);
  auto input_to_forget_weights1 = model->addOperand(&type1);
  auto input_to_cell_weights1 = model->addOperand(&type1);
  auto input_to_output_weights1 = model->addOperand(&type1);
  auto recurrent_to_intput_weights1 = model->addOperand(&type11);
  auto recurrent_to_forget_weights1 = model->addOperand(&type2);
  auto recurrent_to_cell_weights1 = model->addOperand(&type2);
  auto recurrent_to_output_weights1 = model->addOperand(&type2);
  auto cell_to_input_weights1 = model->addOperand(&type5);
  auto cell_to_forget_weights1 = model->addOperand(&type3);
  auto cell_to_output_weights1 = model->addOperand(&type3);
  auto input_gate_bias1 = model->addOperand(&type5);
  auto forget_gate_bias1 = model->addOperand(&type3);
  auto cell_gate_bias1 = model->addOperand(&type3);
  auto output_gate_bias1 = model->addOperand(&type3);
  auto projection_weights1 = model->addOperand(&type4);
  auto projection_bias1 = model->addOperand(&type5);
  auto output_state_in1 = model->addOperand(&type6);
  auto cell_state_in1 = model->addOperand(&type7);
  auto activation_param1 = model->addOperand(&type8);
  auto cell_clip_param1 = model->addOperand(&type9);
  auto proj_clip_param1 = model->addOperand(&type9);
  auto input_layer_norm_weights1 = model->addOperand(&type5);
  auto forget_layer_norm_weights1 = model->addOperand(&type3);
  auto cell_layer_norm_weights1 = model->addOperand(&type3);
  auto output_layer_norm_weights1 = model->addOperand(&type3);
  auto scratch_buffer1 = model->addOperand(&type11);
  auto output_state_out1 = model->addOperand(&type11);
  auto cell_state_out1 = model->addOperand(&type11);
  auto output1 = model->addOperand(&type11);
  // Phase 2, operations
  static int32_t activation_param1_init[] = {4};
  model->setOperandValue(activation_param1, activation_param1_init, sizeof(int32_t) * 1);
  static float cell_clip_param1_init[] = {0.0f};
  model->setOperandValue(cell_clip_param1, cell_clip_param1_init, sizeof(float) * 1);
  static float proj_clip_param1_init[] = {0.0f};
  model->setOperandValue(proj_clip_param1, proj_clip_param1_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_LSTM, {input1, input_to_input_weights1, input_to_forget_weights1, input_to_cell_weights1, input_to_output_weights1, recurrent_to_intput_weights1, recurrent_to_forget_weights1, recurrent_to_cell_weights1, recurrent_to_output_weights1, cell_to_input_weights1, cell_to_forget_weights1, cell_to_output_weights1, input_gate_bias1, forget_gate_bias1, cell_gate_bias1, output_gate_bias1, projection_weights1, projection_bias1, output_state_in1, cell_state_in1, activation_param1, cell_clip_param1, proj_clip_param1, input_layer_norm_weights1, forget_layer_norm_weights1, cell_layer_norm_weights1, output_layer_norm_weights1}, {scratch_buffer1, output_state_out1, cell_state_out1, output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input1, input_to_input_weights1, input_to_forget_weights1, input_to_cell_weights1, input_to_output_weights1, recurrent_to_intput_weights1, recurrent_to_forget_weights1, recurrent_to_cell_weights1, recurrent_to_output_weights1, cell_to_input_weights1, cell_to_forget_weights1, cell_to_output_weights1, input_gate_bias1, forget_gate_bias1, cell_gate_bias1, output_gate_bias1, projection_weights1, projection_bias1, output_state_in1, cell_state_in1, input_layer_norm_weights1, forget_layer_norm_weights1, cell_layer_norm_weights1, output_layer_norm_weights1},
    {scratch_buffer1, output_state_out1, cell_state_out1, output1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_6(int i) {
  static std::set<int> ignore = {0, 1};
  return ignore.find(i) != ignore.end();
}

