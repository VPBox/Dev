// clang-format off
// Generated file (from: bidirectional_sequence_lstm_float16_batch_major_aux_input.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT16, {1, 3, 2});
  OperandType type1(Type::TENSOR_FLOAT16, {4, 2});
  OperandType type2(Type::TENSOR_FLOAT16, {4, 4});
  OperandType type3(Type::TENSOR_FLOAT16, {4});
  OperandType type4(Type::TENSOR_FLOAT16, {1, 4});
  OperandType type5(Type::TENSOR_FLOAT16, {1, 3, 4});
  OperandType type6(Type::INT32, {});
  OperandType type7(Type::FLOAT16, {});
  OperandType type8(Type::BOOL, {});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto fw_input_to_input_weights = model->addOperand(&type1);
  auto fw_input_to_forget_weights = model->addOperand(&type1);
  auto fw_input_to_cell_weights = model->addOperand(&type1);
  auto fw_input_to_output_weights = model->addOperand(&type1);
  auto fw_recurrent_to_input_weights = model->addOperand(&type2);
  auto fw_recurrent_to_forget_weights = model->addOperand(&type2);
  auto fw_recurrent_to_cell_weights = model->addOperand(&type2);
  auto fw_recurrent_to_output_weights = model->addOperand(&type2);
  auto fw_cell_to_input_weights = model->addOperand(&type3);
  auto fw_cell_to_forget_weights = model->addOperand(&type3);
  auto fw_cell_to_output_weights = model->addOperand(&type3);
  auto fw_input_gate_bias = model->addOperand(&type3);
  auto fw_forget_gate_bias = model->addOperand(&type3);
  auto fw_cell_bias = model->addOperand(&type3);
  auto fw_output_gate_bias = model->addOperand(&type3);
  auto fw_projection_weights = model->addOperand(&type2);
  auto fw_projection_bias = model->addOperand(&type3);
  auto bw_input_to_input_weights = model->addOperand(&type1);
  auto bw_input_to_forget_weights = model->addOperand(&type1);
  auto bw_input_to_cell_weights = model->addOperand(&type1);
  auto bw_input_to_output_weights = model->addOperand(&type1);
  auto bw_recurrent_to_input_weights = model->addOperand(&type2);
  auto bw_recurrent_to_forget_weights = model->addOperand(&type2);
  auto bw_recurrent_to_cell_weights = model->addOperand(&type2);
  auto bw_recurrent_to_output_weights = model->addOperand(&type2);
  auto bw_cell_to_input_weights = model->addOperand(&type3);
  auto bw_cell_to_forget_weights = model->addOperand(&type3);
  auto bw_cell_to_output_weights = model->addOperand(&type3);
  auto bw_input_gate_bias = model->addOperand(&type3);
  auto bw_forget_gate_bias = model->addOperand(&type3);
  auto bw_cell_bias = model->addOperand(&type3);
  auto bw_output_gate_bias = model->addOperand(&type3);
  auto bw_projection_weights = model->addOperand(&type2);
  auto bw_projection_bias = model->addOperand(&type3);
  auto fw_activatiom_state = model->addOperand(&type4);
  auto fw_cell_state = model->addOperand(&type4);
  auto bw_activatiom_state = model->addOperand(&type4);
  auto bw_cell_state = model->addOperand(&type4);
  auto input1 = model->addOperand(&type0);
  auto fw_aux_input_to_input_weights = model->addOperand(&type1);
  auto fw_input_to_forget_weights1 = model->addOperand(&type1);
  auto fw_aux_input_to_cell_weights = model->addOperand(&type1);
  auto fw_aux_input_to_output_weights = model->addOperand(&type1);
  auto bw_aux_input_to_input_weights = model->addOperand(&type1);
  auto bw_input_to_forget_weights1 = model->addOperand(&type1);
  auto bw_aux_input_to_cell_weights = model->addOperand(&type1);
  auto bw_aux_input_to_output_weights = model->addOperand(&type1);
  auto activation = model->addOperand(&type6);
  auto cell_clip = model->addOperand(&type7);
  auto proj_clip = model->addOperand(&type7);
  auto merge_outputs = model->addOperand(&type8);
  auto time_major = model->addOperand(&type8);
  auto input_layer_norm_weights = model->addOperand(&type3);
  auto forget_layer_norm_weights = model->addOperand(&type3);
  auto cell_layer_norm_weights = model->addOperand(&type3);
  auto output_layer_norm_weights = model->addOperand(&type3);
  auto input_layer_norm_weights1 = model->addOperand(&type3);
  auto forget_layer_norm_weights1 = model->addOperand(&type3);
  auto cell_layer_norm_weights1 = model->addOperand(&type3);
  auto output_layer_norm_weights1 = model->addOperand(&type3);
  auto fw_output = model->addOperand(&type5);
  auto bw_output = model->addOperand(&type5);
  // Phase 2, operations
  static int32_t activation_init[] = {4};
  model->setOperandValue(activation, activation_init, sizeof(int32_t) * 1);
  static _Float16 cell_clip_init[] = {0.0f};
  model->setOperandValue(cell_clip, cell_clip_init, sizeof(_Float16) * 1);
  static _Float16 proj_clip_init[] = {0.0f};
  model->setOperandValue(proj_clip, proj_clip_init, sizeof(_Float16) * 1);
  static bool8 merge_outputs_init[] = {false};
  model->setOperandValue(merge_outputs, merge_outputs_init, sizeof(bool8) * 1);
  static bool8 time_major_init[] = {false};
  model->setOperandValue(time_major, time_major_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_LSTM, {input, fw_input_to_input_weights, fw_input_to_forget_weights, fw_input_to_cell_weights, fw_input_to_output_weights, fw_recurrent_to_input_weights, fw_recurrent_to_forget_weights, fw_recurrent_to_cell_weights, fw_recurrent_to_output_weights, fw_cell_to_input_weights, fw_cell_to_forget_weights, fw_cell_to_output_weights, fw_input_gate_bias, fw_forget_gate_bias, fw_cell_bias, fw_output_gate_bias, fw_projection_weights, fw_projection_bias, bw_input_to_input_weights, bw_input_to_forget_weights, bw_input_to_cell_weights, bw_input_to_output_weights, bw_recurrent_to_input_weights, bw_recurrent_to_forget_weights, bw_recurrent_to_cell_weights, bw_recurrent_to_output_weights, bw_cell_to_input_weights, bw_cell_to_forget_weights, bw_cell_to_output_weights, bw_input_gate_bias, bw_forget_gate_bias, bw_cell_bias, bw_output_gate_bias, bw_projection_weights, bw_projection_bias, fw_activatiom_state, fw_cell_state, bw_activatiom_state, bw_cell_state, input1, fw_aux_input_to_input_weights, fw_input_to_forget_weights1, fw_aux_input_to_cell_weights, fw_aux_input_to_output_weights, bw_aux_input_to_input_weights, bw_input_to_forget_weights1, bw_aux_input_to_cell_weights, bw_aux_input_to_output_weights, activation, cell_clip, proj_clip, merge_outputs, time_major, input_layer_norm_weights, forget_layer_norm_weights, cell_layer_norm_weights, output_layer_norm_weights, input_layer_norm_weights1, forget_layer_norm_weights1, cell_layer_norm_weights1, output_layer_norm_weights1}, {fw_output, bw_output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, fw_input_to_input_weights, fw_input_to_forget_weights, fw_input_to_cell_weights, fw_input_to_output_weights, fw_recurrent_to_input_weights, fw_recurrent_to_forget_weights, fw_recurrent_to_cell_weights, fw_recurrent_to_output_weights, fw_cell_to_input_weights, fw_cell_to_forget_weights, fw_cell_to_output_weights, fw_input_gate_bias, fw_forget_gate_bias, fw_cell_bias, fw_output_gate_bias, fw_projection_weights, fw_projection_bias, bw_input_to_input_weights, bw_input_to_forget_weights, bw_input_to_cell_weights, bw_input_to_output_weights, bw_recurrent_to_input_weights, bw_recurrent_to_forget_weights, bw_recurrent_to_cell_weights, bw_recurrent_to_output_weights, bw_cell_to_input_weights, bw_cell_to_forget_weights, bw_cell_to_output_weights, bw_input_gate_bias, bw_forget_gate_bias, bw_cell_bias, bw_output_gate_bias, bw_projection_weights, bw_projection_bias, fw_activatiom_state, fw_cell_state, bw_activatiom_state, bw_cell_state, input1, fw_aux_input_to_input_weights, fw_input_to_forget_weights1, fw_aux_input_to_cell_weights, fw_aux_input_to_output_weights, bw_aux_input_to_input_weights, bw_input_to_forget_weights1, bw_aux_input_to_cell_weights, bw_aux_input_to_output_weights, input_layer_norm_weights, forget_layer_norm_weights, cell_layer_norm_weights, output_layer_norm_weights, input_layer_norm_weights1, forget_layer_norm_weights1, cell_layer_norm_weights1, output_layer_norm_weights1},
    {fw_output, bw_output});
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT16, {1, 3, 2});
  OperandType type1(Type::TENSOR_FLOAT16, {4, 2});
  OperandType type2(Type::TENSOR_FLOAT16, {4, 4});
  OperandType type3(Type::TENSOR_FLOAT16, {4});
  OperandType type4(Type::TENSOR_FLOAT16, {1, 4});
  OperandType type6(Type::INT32, {});
  OperandType type7(Type::FLOAT16, {});
  OperandType type8(Type::BOOL, {});
  OperandType type9(Type::TENSOR_FLOAT16, {0, 0, 0});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto fw_input_to_input_weights = model->addOperand(&type1);
  auto fw_input_to_forget_weights = model->addOperand(&type1);
  auto fw_input_to_cell_weights = model->addOperand(&type1);
  auto fw_input_to_output_weights = model->addOperand(&type1);
  auto fw_recurrent_to_input_weights = model->addOperand(&type2);
  auto fw_recurrent_to_forget_weights = model->addOperand(&type2);
  auto fw_recurrent_to_cell_weights = model->addOperand(&type2);
  auto fw_recurrent_to_output_weights = model->addOperand(&type2);
  auto fw_cell_to_input_weights = model->addOperand(&type3);
  auto fw_cell_to_forget_weights = model->addOperand(&type3);
  auto fw_cell_to_output_weights = model->addOperand(&type3);
  auto fw_input_gate_bias = model->addOperand(&type3);
  auto fw_forget_gate_bias = model->addOperand(&type3);
  auto fw_cell_bias = model->addOperand(&type3);
  auto fw_output_gate_bias = model->addOperand(&type3);
  auto fw_projection_weights = model->addOperand(&type2);
  auto fw_projection_bias = model->addOperand(&type3);
  auto bw_input_to_input_weights = model->addOperand(&type1);
  auto bw_input_to_forget_weights = model->addOperand(&type1);
  auto bw_input_to_cell_weights = model->addOperand(&type1);
  auto bw_input_to_output_weights = model->addOperand(&type1);
  auto bw_recurrent_to_input_weights = model->addOperand(&type2);
  auto bw_recurrent_to_forget_weights = model->addOperand(&type2);
  auto bw_recurrent_to_cell_weights = model->addOperand(&type2);
  auto bw_recurrent_to_output_weights = model->addOperand(&type2);
  auto bw_cell_to_input_weights = model->addOperand(&type3);
  auto bw_cell_to_forget_weights = model->addOperand(&type3);
  auto bw_cell_to_output_weights = model->addOperand(&type3);
  auto bw_input_gate_bias = model->addOperand(&type3);
  auto bw_forget_gate_bias = model->addOperand(&type3);
  auto bw_cell_bias = model->addOperand(&type3);
  auto bw_output_gate_bias = model->addOperand(&type3);
  auto bw_projection_weights = model->addOperand(&type2);
  auto bw_projection_bias = model->addOperand(&type3);
  auto fw_activatiom_state = model->addOperand(&type4);
  auto fw_cell_state = model->addOperand(&type4);
  auto bw_activatiom_state = model->addOperand(&type4);
  auto bw_cell_state = model->addOperand(&type4);
  auto input1 = model->addOperand(&type0);
  auto fw_aux_input_to_input_weights = model->addOperand(&type1);
  auto fw_input_to_forget_weights1 = model->addOperand(&type1);
  auto fw_aux_input_to_cell_weights = model->addOperand(&type1);
  auto fw_aux_input_to_output_weights = model->addOperand(&type1);
  auto bw_aux_input_to_input_weights = model->addOperand(&type1);
  auto bw_input_to_forget_weights1 = model->addOperand(&type1);
  auto bw_aux_input_to_cell_weights = model->addOperand(&type1);
  auto bw_aux_input_to_output_weights = model->addOperand(&type1);
  auto activation = model->addOperand(&type6);
  auto cell_clip = model->addOperand(&type7);
  auto proj_clip = model->addOperand(&type7);
  auto merge_outputs = model->addOperand(&type8);
  auto time_major = model->addOperand(&type8);
  auto input_layer_norm_weights = model->addOperand(&type3);
  auto forget_layer_norm_weights = model->addOperand(&type3);
  auto cell_layer_norm_weights = model->addOperand(&type3);
  auto output_layer_norm_weights = model->addOperand(&type3);
  auto input_layer_norm_weights1 = model->addOperand(&type3);
  auto forget_layer_norm_weights1 = model->addOperand(&type3);
  auto cell_layer_norm_weights1 = model->addOperand(&type3);
  auto output_layer_norm_weights1 = model->addOperand(&type3);
  auto fw_output = model->addOperand(&type9);
  auto bw_output = model->addOperand(&type9);
  // Phase 2, operations
  static int32_t activation_init[] = {4};
  model->setOperandValue(activation, activation_init, sizeof(int32_t) * 1);
  static _Float16 cell_clip_init[] = {0.0f};
  model->setOperandValue(cell_clip, cell_clip_init, sizeof(_Float16) * 1);
  static _Float16 proj_clip_init[] = {0.0f};
  model->setOperandValue(proj_clip, proj_clip_init, sizeof(_Float16) * 1);
  static bool8 merge_outputs_init[] = {false};
  model->setOperandValue(merge_outputs, merge_outputs_init, sizeof(bool8) * 1);
  static bool8 time_major_init[] = {false};
  model->setOperandValue(time_major, time_major_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BIDIRECTIONAL_SEQUENCE_LSTM, {input, fw_input_to_input_weights, fw_input_to_forget_weights, fw_input_to_cell_weights, fw_input_to_output_weights, fw_recurrent_to_input_weights, fw_recurrent_to_forget_weights, fw_recurrent_to_cell_weights, fw_recurrent_to_output_weights, fw_cell_to_input_weights, fw_cell_to_forget_weights, fw_cell_to_output_weights, fw_input_gate_bias, fw_forget_gate_bias, fw_cell_bias, fw_output_gate_bias, fw_projection_weights, fw_projection_bias, bw_input_to_input_weights, bw_input_to_forget_weights, bw_input_to_cell_weights, bw_input_to_output_weights, bw_recurrent_to_input_weights, bw_recurrent_to_forget_weights, bw_recurrent_to_cell_weights, bw_recurrent_to_output_weights, bw_cell_to_input_weights, bw_cell_to_forget_weights, bw_cell_to_output_weights, bw_input_gate_bias, bw_forget_gate_bias, bw_cell_bias, bw_output_gate_bias, bw_projection_weights, bw_projection_bias, fw_activatiom_state, fw_cell_state, bw_activatiom_state, bw_cell_state, input1, fw_aux_input_to_input_weights, fw_input_to_forget_weights1, fw_aux_input_to_cell_weights, fw_aux_input_to_output_weights, bw_aux_input_to_input_weights, bw_input_to_forget_weights1, bw_aux_input_to_cell_weights, bw_aux_input_to_output_weights, activation, cell_clip, proj_clip, merge_outputs, time_major, input_layer_norm_weights, forget_layer_norm_weights, cell_layer_norm_weights, output_layer_norm_weights, input_layer_norm_weights1, forget_layer_norm_weights1, cell_layer_norm_weights1, output_layer_norm_weights1}, {fw_output, bw_output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, fw_input_to_input_weights, fw_input_to_forget_weights, fw_input_to_cell_weights, fw_input_to_output_weights, fw_recurrent_to_input_weights, fw_recurrent_to_forget_weights, fw_recurrent_to_cell_weights, fw_recurrent_to_output_weights, fw_cell_to_input_weights, fw_cell_to_forget_weights, fw_cell_to_output_weights, fw_input_gate_bias, fw_forget_gate_bias, fw_cell_bias, fw_output_gate_bias, fw_projection_weights, fw_projection_bias, bw_input_to_input_weights, bw_input_to_forget_weights, bw_input_to_cell_weights, bw_input_to_output_weights, bw_recurrent_to_input_weights, bw_recurrent_to_forget_weights, bw_recurrent_to_cell_weights, bw_recurrent_to_output_weights, bw_cell_to_input_weights, bw_cell_to_forget_weights, bw_cell_to_output_weights, bw_input_gate_bias, bw_forget_gate_bias, bw_cell_bias, bw_output_gate_bias, bw_projection_weights, bw_projection_bias, fw_activatiom_state, fw_cell_state, bw_activatiom_state, bw_cell_state, input1, fw_aux_input_to_input_weights, fw_input_to_forget_weights1, fw_aux_input_to_cell_weights, fw_aux_input_to_output_weights, bw_aux_input_to_input_weights, bw_input_to_forget_weights1, bw_aux_input_to_cell_weights, bw_aux_input_to_output_weights, input_layer_norm_weights, forget_layer_norm_weights, cell_layer_norm_weights, output_layer_norm_weights, input_layer_norm_weights1, forget_layer_norm_weights1, cell_layer_norm_weights1, output_layer_norm_weights1},
    {fw_output, bw_output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

