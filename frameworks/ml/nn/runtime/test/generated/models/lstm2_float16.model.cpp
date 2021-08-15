// clang-format off
// Generated file (from: lstm2_float16.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type1(Type::TENSOR_FLOAT16, {4, 2});
  OperandType type2(Type::TENSOR_FLOAT16, {4, 4});
  OperandType type3(Type::TENSOR_FLOAT16, {0});
  OperandType type4(Type::TENSOR_FLOAT16, {4});
  OperandType type5(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type6(Type::TENSOR_FLOAT16, {1, 4});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::FLOAT16, {});
  OperandType type9(Type::TENSOR_FLOAT16, {1, 12});
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
  auto cell_to_forget_weights = model->addOperand(&type4);
  auto cell_to_output_weights = model->addOperand(&type4);
  auto input_gate_bias = model->addOperand(&type4);
  auto forget_gate_bias = model->addOperand(&type4);
  auto cell_gate_bias = model->addOperand(&type4);
  auto output_gate_bias = model->addOperand(&type4);
  auto projection_weights = model->addOperand(&type5);
  auto projection_bias = model->addOperand(&type3);
  auto output_state_in = model->addOperand(&type6);
  auto cell_state_in = model->addOperand(&type6);
  auto activation_param = model->addOperand(&type7);
  auto cell_clip_param = model->addOperand(&type8);
  auto proj_clip_param = model->addOperand(&type8);
  auto scratch_buffer = model->addOperand(&type9);
  auto output_state_out = model->addOperand(&type6);
  auto cell_state_out = model->addOperand(&type6);
  auto output = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t activation_param_init[] = {4};
  model->setOperandValue(activation_param, activation_param_init, sizeof(int32_t) * 1);
  static _Float16 cell_clip_param_init[] = {0.0f};
  model->setOperandValue(cell_clip_param, cell_clip_param_init, sizeof(_Float16) * 1);
  static _Float16 proj_clip_param_init[] = {0.0f};
  model->setOperandValue(proj_clip_param, proj_clip_param_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_LSTM, {input, input_to_input_weights, input_to_forget_weights, input_to_cell_weights, input_to_output_weights, recurrent_to_intput_weights, recurrent_to_forget_weights, recurrent_to_cell_weights, recurrent_to_output_weights, cell_to_input_weights, cell_to_forget_weights, cell_to_output_weights, input_gate_bias, forget_gate_bias, cell_gate_bias, output_gate_bias, projection_weights, projection_bias, output_state_in, cell_state_in, activation_param, cell_clip_param, proj_clip_param}, {scratch_buffer, output_state_out, cell_state_out, output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, input_to_input_weights, input_to_forget_weights, input_to_cell_weights, input_to_output_weights, recurrent_to_intput_weights, recurrent_to_forget_weights, recurrent_to_cell_weights, recurrent_to_output_weights, cell_to_input_weights, cell_to_forget_weights, cell_to_output_weights, input_gate_bias, forget_gate_bias, cell_gate_bias, output_gate_bias, projection_weights, projection_bias, output_state_in, cell_state_in},
    {scratch_buffer, output_state_out, cell_state_out, output});
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {0};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type1(Type::TENSOR_FLOAT16, {4, 2});
  OperandType type2(Type::TENSOR_FLOAT16, {4, 4});
  OperandType type3(Type::TENSOR_FLOAT16, {0});
  OperandType type4(Type::TENSOR_FLOAT16, {4});
  OperandType type5(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type6(Type::TENSOR_FLOAT16, {1, 4});
  OperandType type7(Type::INT32, {});
  OperandType type8(Type::FLOAT16, {});
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
  auto cell_to_forget_weights = model->addOperand(&type4);
  auto cell_to_output_weights = model->addOperand(&type4);
  auto input_gate_bias = model->addOperand(&type4);
  auto forget_gate_bias = model->addOperand(&type4);
  auto cell_gate_bias = model->addOperand(&type4);
  auto output_gate_bias = model->addOperand(&type4);
  auto projection_weights = model->addOperand(&type5);
  auto projection_bias = model->addOperand(&type3);
  auto output_state_in = model->addOperand(&type6);
  auto cell_state_in = model->addOperand(&type6);
  auto activation_param = model->addOperand(&type7);
  auto cell_clip_param = model->addOperand(&type8);
  auto proj_clip_param = model->addOperand(&type8);
  auto scratch_buffer = model->addOperand(&type5);
  auto output_state_out = model->addOperand(&type5);
  auto cell_state_out = model->addOperand(&type5);
  auto output = model->addOperand(&type5);
  // Phase 2, operations
  static int32_t activation_param_init[] = {4};
  model->setOperandValue(activation_param, activation_param_init, sizeof(int32_t) * 1);
  static _Float16 cell_clip_param_init[] = {0.0f};
  model->setOperandValue(cell_clip_param, cell_clip_param_init, sizeof(_Float16) * 1);
  static _Float16 proj_clip_param_init[] = {0.0f};
  model->setOperandValue(proj_clip_param, proj_clip_param_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_LSTM, {input, input_to_input_weights, input_to_forget_weights, input_to_cell_weights, input_to_output_weights, recurrent_to_intput_weights, recurrent_to_forget_weights, recurrent_to_cell_weights, recurrent_to_output_weights, cell_to_input_weights, cell_to_forget_weights, cell_to_output_weights, input_gate_bias, forget_gate_bias, cell_gate_bias, output_gate_bias, projection_weights, projection_bias, output_state_in, cell_state_in, activation_param, cell_clip_param, proj_clip_param}, {scratch_buffer, output_state_out, cell_state_out, output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, input_to_input_weights, input_to_forget_weights, input_to_cell_weights, input_to_output_weights, recurrent_to_intput_weights, recurrent_to_forget_weights, recurrent_to_cell_weights, recurrent_to_output_weights, cell_to_input_weights, cell_to_forget_weights, cell_to_output_weights, input_gate_bias, forget_gate_bias, cell_gate_bias, output_gate_bias, projection_weights, projection_bias, output_state_in, cell_state_in},
    {scratch_buffer, output_state_out, cell_state_out, output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {0};
  return ignore.find(i) != ignore.end();
}

