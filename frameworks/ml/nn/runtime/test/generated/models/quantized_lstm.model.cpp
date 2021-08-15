// clang-format off
// Generated file (from: quantized_lstm.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {2, 2}, 0.0078125f, 128);
  OperandType type1(Type::TENSOR_QUANT8_ASYMM, {4, 2}, 0.00408021f, 100);
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {4, 4}, 0.00408021f, 100);
  OperandType type3(Type::TENSOR_INT32, {4}, 3.1876640625e-05f, 0);
  OperandType type4(Type::TENSOR_QUANT16_SYMM, {2, 4}, 0.00048828125f, 0);
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {2, 4}, 0.0078125f, 128);
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto inputToInputWeights = model->addOperand(&type1);
  auto inputToForgetWeights = model->addOperand(&type1);
  auto inputToCellWeights = model->addOperand(&type1);
  auto inputToOutputWeights = model->addOperand(&type1);
  auto recurrentToInputWeights = model->addOperand(&type2);
  auto recurrentToForgetWeights = model->addOperand(&type2);
  auto recurrentToCellWeights = model->addOperand(&type2);
  auto recurrentToOutputWeights = model->addOperand(&type2);
  auto inputGateBias = model->addOperand(&type3);
  auto forgetGateBias = model->addOperand(&type3);
  auto cellGateBias = model->addOperand(&type3);
  auto outputGateBias = model->addOperand(&type3);
  auto prevCellState = model->addOperand(&type4);
  auto prevOutput = model->addOperand(&type5);
  auto cellStateOut = model->addOperand(&type4);
  auto output = model->addOperand(&type5);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_QUANTIZED_16BIT_LSTM, {input, inputToInputWeights, inputToForgetWeights, inputToCellWeights, inputToOutputWeights, recurrentToInputWeights, recurrentToForgetWeights, recurrentToCellWeights, recurrentToOutputWeights, inputGateBias, forgetGateBias, cellGateBias, outputGateBias, prevCellState, prevOutput}, {cellStateOut, output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, inputToInputWeights, inputToForgetWeights, inputToCellWeights, inputToOutputWeights, recurrentToInputWeights, recurrentToForgetWeights, recurrentToCellWeights, recurrentToOutputWeights, inputGateBias, forgetGateBias, cellGateBias, outputGateBias, prevCellState, prevOutput},
    {cellStateOut, output});
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {2, 2}, 0.0078125f, 128);
  OperandType type1(Type::TENSOR_QUANT8_ASYMM, {4, 2}, 0.00408021f, 100);
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {4, 4}, 0.00408021f, 100);
  OperandType type3(Type::TENSOR_INT32, {4}, 3.1876640625e-05f, 0);
  OperandType type4(Type::TENSOR_QUANT16_SYMM, {2, 4}, 0.00048828125f, 0);
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {2, 4}, 0.0078125f, 128);
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto inputToInputWeights = model->addOperand(&type1);
  auto inputToForgetWeights = model->addOperand(&type1);
  auto inputToCellWeights = model->addOperand(&type1);
  auto inputToOutputWeights = model->addOperand(&type1);
  auto recurrentToInputWeights = model->addOperand(&type2);
  auto recurrentToForgetWeights = model->addOperand(&type2);
  auto recurrentToCellWeights = model->addOperand(&type2);
  auto recurrentToOutputWeights = model->addOperand(&type2);
  auto inputGateBias = model->addOperand(&type3);
  auto forgetGateBias = model->addOperand(&type3);
  auto cellGateBias = model->addOperand(&type3);
  auto outputGateBias = model->addOperand(&type3);
  auto prevCellState = model->addOperand(&type4);
  auto prevOutput = model->addOperand(&type5);
  auto cellStateOut = model->addOperand(&type4);
  auto output = model->addOperand(&type5);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_QUANTIZED_16BIT_LSTM, {input, inputToInputWeights, inputToForgetWeights, inputToCellWeights, inputToOutputWeights, recurrentToInputWeights, recurrentToForgetWeights, recurrentToCellWeights, recurrentToOutputWeights, inputGateBias, forgetGateBias, cellGateBias, outputGateBias, prevCellState, prevOutput}, {cellStateOut, output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, inputToInputWeights, inputToForgetWeights, inputToCellWeights, inputToOutputWeights, recurrentToInputWeights, recurrentToForgetWeights, recurrentToCellWeights, recurrentToOutputWeights, inputGateBias, forgetGateBias, cellGateBias, outputGateBias, prevCellState, prevOutput},
    {cellStateOut, output});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {2, 2}, 0.0078125f, 128);
  OperandType type1(Type::TENSOR_QUANT8_ASYMM, {4, 2}, 0.00408021f, 100);
  OperandType type10(Type::TENSOR_QUANT8_ASYMM, {0, 0}, 0.0078125f, 128);
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {4, 4}, 0.00408021f, 100);
  OperandType type3(Type::TENSOR_INT32, {4}, 3.1876640625e-05f, 0);
  OperandType type4(Type::TENSOR_QUANT16_SYMM, {2, 4}, 0.00048828125f, 0);
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {2, 4}, 0.0078125f, 128);
  OperandType type9(Type::TENSOR_QUANT16_SYMM, {0, 0}, 0.00048828125f, 0);
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto inputToInputWeights = model->addOperand(&type1);
  auto inputToForgetWeights = model->addOperand(&type1);
  auto inputToCellWeights = model->addOperand(&type1);
  auto inputToOutputWeights = model->addOperand(&type1);
  auto recurrentToInputWeights = model->addOperand(&type2);
  auto recurrentToForgetWeights = model->addOperand(&type2);
  auto recurrentToCellWeights = model->addOperand(&type2);
  auto recurrentToOutputWeights = model->addOperand(&type2);
  auto inputGateBias = model->addOperand(&type3);
  auto forgetGateBias = model->addOperand(&type3);
  auto cellGateBias = model->addOperand(&type3);
  auto outputGateBias = model->addOperand(&type3);
  auto prevCellState = model->addOperand(&type4);
  auto prevOutput = model->addOperand(&type5);
  auto cellStateOut = model->addOperand(&type9);
  auto output = model->addOperand(&type10);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_QUANTIZED_16BIT_LSTM, {input, inputToInputWeights, inputToForgetWeights, inputToCellWeights, inputToOutputWeights, recurrentToInputWeights, recurrentToForgetWeights, recurrentToCellWeights, recurrentToOutputWeights, inputGateBias, forgetGateBias, cellGateBias, outputGateBias, prevCellState, prevOutput}, {cellStateOut, output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, inputToInputWeights, inputToForgetWeights, inputToCellWeights, inputToOutputWeights, recurrentToInputWeights, recurrentToForgetWeights, recurrentToCellWeights, recurrentToOutputWeights, inputGateBias, forgetGateBias, cellGateBias, outputGateBias, prevCellState, prevOutput},
    {cellStateOut, output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {2, 2}, 0.0078125f, 128);
  OperandType type1(Type::TENSOR_QUANT8_ASYMM, {4, 2}, 0.00408021f, 100);
  OperandType type10(Type::TENSOR_QUANT8_ASYMM, {0, 0}, 0.0078125f, 128);
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {4, 4}, 0.00408021f, 100);
  OperandType type3(Type::TENSOR_INT32, {4}, 3.1876640625e-05f, 0);
  OperandType type4(Type::TENSOR_QUANT16_SYMM, {2, 4}, 0.00048828125f, 0);
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {2, 4}, 0.0078125f, 128);
  OperandType type9(Type::TENSOR_QUANT16_SYMM, {0, 0}, 0.00048828125f, 0);
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto inputToInputWeights = model->addOperand(&type1);
  auto inputToForgetWeights = model->addOperand(&type1);
  auto inputToCellWeights = model->addOperand(&type1);
  auto inputToOutputWeights = model->addOperand(&type1);
  auto recurrentToInputWeights = model->addOperand(&type2);
  auto recurrentToForgetWeights = model->addOperand(&type2);
  auto recurrentToCellWeights = model->addOperand(&type2);
  auto recurrentToOutputWeights = model->addOperand(&type2);
  auto inputGateBias = model->addOperand(&type3);
  auto forgetGateBias = model->addOperand(&type3);
  auto cellGateBias = model->addOperand(&type3);
  auto outputGateBias = model->addOperand(&type3);
  auto prevCellState = model->addOperand(&type4);
  auto prevOutput = model->addOperand(&type5);
  auto cellStateOut = model->addOperand(&type9);
  auto output = model->addOperand(&type10);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_QUANTIZED_16BIT_LSTM, {input, inputToInputWeights, inputToForgetWeights, inputToCellWeights, inputToOutputWeights, recurrentToInputWeights, recurrentToForgetWeights, recurrentToCellWeights, recurrentToOutputWeights, inputGateBias, forgetGateBias, cellGateBias, outputGateBias, prevCellState, prevOutput}, {cellStateOut, output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, inputToInputWeights, inputToForgetWeights, inputToCellWeights, inputToOutputWeights, recurrentToInputWeights, recurrentToForgetWeights, recurrentToCellWeights, recurrentToOutputWeights, inputGateBias, forgetGateBias, cellGateBias, outputGateBias, prevCellState, prevOutput},
    {cellStateOut, output});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_2(Model *model) {
  OperandType type1(Type::TENSOR_QUANT8_ASYMM, {4, 2}, 0.00408021f, 100);
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {4, 4}, 0.00408021f, 100);
  OperandType type3(Type::TENSOR_INT32, {4}, 3.1876640625e-05f, 0);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.0078125f, 128);
  OperandType type7(Type::TENSOR_QUANT16_SYMM, {1, 4}, 0.00048828125f, 0);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1, 4}, 0.0078125f, 128);
  // Phase 1, operands
  auto input1 = model->addOperand(&type6);
  auto inputToInputWeights1 = model->addOperand(&type1);
  auto inputToForgetWeights1 = model->addOperand(&type1);
  auto inputToCellWeights1 = model->addOperand(&type1);
  auto inputToOutputWeights1 = model->addOperand(&type1);
  auto recurrentToInputWeights1 = model->addOperand(&type2);
  auto recurrentToForgetWeights1 = model->addOperand(&type2);
  auto recurrentToCellWeights1 = model->addOperand(&type2);
  auto recurrentToOutputWeights1 = model->addOperand(&type2);
  auto inputGateBias1 = model->addOperand(&type3);
  auto forgetGateBias1 = model->addOperand(&type3);
  auto cellGateBias1 = model->addOperand(&type3);
  auto outputGateBias1 = model->addOperand(&type3);
  auto prevCellState1 = model->addOperand(&type7);
  auto prevOutput1 = model->addOperand(&type8);
  auto cellStateOut1 = model->addOperand(&type7);
  auto output1 = model->addOperand(&type8);
  // Phase 2, operations
  static uint8_t inputToInputWeights1_init[] = {146, 250, 235, 171, 10, 218, 171, 108};
  model->setOperandValue(inputToInputWeights1, inputToInputWeights1_init, sizeof(uint8_t) * 8);
  static uint8_t inputToForgetWeights1_init[] = {24, 50, 132, 179, 158, 110, 3, 169};
  model->setOperandValue(inputToForgetWeights1, inputToForgetWeights1_init, sizeof(uint8_t) * 8);
  static uint8_t inputToCellWeights1_init[] = {133, 34, 29, 49, 206, 109, 54, 183};
  model->setOperandValue(inputToCellWeights1, inputToCellWeights1_init, sizeof(uint8_t) * 8);
  static uint8_t inputToOutputWeights1_init[] = {195, 187, 11, 99, 109, 10, 218, 48};
  model->setOperandValue(inputToOutputWeights1, inputToOutputWeights1_init, sizeof(uint8_t) * 8);
  static uint8_t recurrentToInputWeights1_init[] = {254, 206, 77, 168, 71, 20, 215, 6, 223, 7, 118, 225, 59, 130, 174, 26};
  model->setOperandValue(recurrentToInputWeights1, recurrentToInputWeights1_init, sizeof(uint8_t) * 16);
  static uint8_t recurrentToForgetWeights1_init[] = {137, 240, 103, 52, 68, 51, 237, 112, 0, 220, 89, 23, 69, 4, 207, 253};
  model->setOperandValue(recurrentToForgetWeights1, recurrentToForgetWeights1_init, sizeof(uint8_t) * 16);
  static uint8_t recurrentToCellWeights1_init[] = {172, 60, 205, 65, 14, 0, 140, 168, 240, 223, 133, 56, 142, 64, 246, 216};
  model->setOperandValue(recurrentToCellWeights1, recurrentToCellWeights1_init, sizeof(uint8_t) * 16);
  static uint8_t recurrentToOutputWeights1_init[] = {106, 214, 67, 23, 59, 158, 45, 3, 119, 132, 49, 205, 129, 218, 11, 98};
  model->setOperandValue(recurrentToOutputWeights1, recurrentToOutputWeights1_init, sizeof(uint8_t) * 16);
  static int32_t inputGateBias1_init[] = {-7876, 13488, -726, 32839};
  model->setOperandValue(inputGateBias1, inputGateBias1_init, sizeof(int32_t) * 4);
  static int32_t forgetGateBias1_init[] = {9206, -46884, -11693, -38724};
  model->setOperandValue(forgetGateBias1, forgetGateBias1_init, sizeof(int32_t) * 4);
  static int32_t cellGateBias1_init[] = {39481, 48624, 48976, -21419};
  model->setOperandValue(cellGateBias1, cellGateBias1_init, sizeof(int32_t) * 4);
  static int32_t outputGateBias1_init[] = {-58999, -17050, -41852, -40538};
  model->setOperandValue(outputGateBias1, outputGateBias1_init, sizeof(int32_t) * 4);
  model->addOperation(ANEURALNETWORKS_QUANTIZED_16BIT_LSTM, {input1, inputToInputWeights1, inputToForgetWeights1, inputToCellWeights1, inputToOutputWeights1, recurrentToInputWeights1, recurrentToForgetWeights1, recurrentToCellWeights1, recurrentToOutputWeights1, inputGateBias1, forgetGateBias1, cellGateBias1, outputGateBias1, prevCellState1, prevOutput1}, {cellStateOut1, output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input1, prevCellState1, prevOutput1},
    {cellStateOut1, output1});
  assert(model->isValid());
}

inline bool is_ignored_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_2(Model *model) {
  OperandType type1(Type::TENSOR_QUANT8_ASYMM, {4, 2}, 0.00408021f, 100);
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {4, 4}, 0.00408021f, 100);
  OperandType type3(Type::TENSOR_INT32, {4}, 3.1876640625e-05f, 0);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.0078125f, 128);
  OperandType type7(Type::TENSOR_QUANT16_SYMM, {1, 4}, 0.00048828125f, 0);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1, 4}, 0.0078125f, 128);
  // Phase 1, operands
  auto input1 = model->addOperand(&type6);
  auto inputToInputWeights1 = model->addOperand(&type1);
  auto inputToForgetWeights1 = model->addOperand(&type1);
  auto inputToCellWeights1 = model->addOperand(&type1);
  auto inputToOutputWeights1 = model->addOperand(&type1);
  auto recurrentToInputWeights1 = model->addOperand(&type2);
  auto recurrentToForgetWeights1 = model->addOperand(&type2);
  auto recurrentToCellWeights1 = model->addOperand(&type2);
  auto recurrentToOutputWeights1 = model->addOperand(&type2);
  auto inputGateBias1 = model->addOperand(&type3);
  auto forgetGateBias1 = model->addOperand(&type3);
  auto cellGateBias1 = model->addOperand(&type3);
  auto outputGateBias1 = model->addOperand(&type3);
  auto prevCellState1 = model->addOperand(&type7);
  auto prevOutput1 = model->addOperand(&type8);
  auto cellStateOut1 = model->addOperand(&type7);
  auto output1 = model->addOperand(&type8);
  // Phase 2, operations
  static uint8_t inputToInputWeights1_init[] = {146, 250, 235, 171, 10, 218, 171, 108};
  model->setOperandValue(inputToInputWeights1, inputToInputWeights1_init, sizeof(uint8_t) * 8);
  static uint8_t inputToForgetWeights1_init[] = {24, 50, 132, 179, 158, 110, 3, 169};
  model->setOperandValue(inputToForgetWeights1, inputToForgetWeights1_init, sizeof(uint8_t) * 8);
  static uint8_t inputToCellWeights1_init[] = {133, 34, 29, 49, 206, 109, 54, 183};
  model->setOperandValue(inputToCellWeights1, inputToCellWeights1_init, sizeof(uint8_t) * 8);
  static uint8_t inputToOutputWeights1_init[] = {195, 187, 11, 99, 109, 10, 218, 48};
  model->setOperandValue(inputToOutputWeights1, inputToOutputWeights1_init, sizeof(uint8_t) * 8);
  static uint8_t recurrentToInputWeights1_init[] = {254, 206, 77, 168, 71, 20, 215, 6, 223, 7, 118, 225, 59, 130, 174, 26};
  model->setOperandValue(recurrentToInputWeights1, recurrentToInputWeights1_init, sizeof(uint8_t) * 16);
  static uint8_t recurrentToForgetWeights1_init[] = {137, 240, 103, 52, 68, 51, 237, 112, 0, 220, 89, 23, 69, 4, 207, 253};
  model->setOperandValue(recurrentToForgetWeights1, recurrentToForgetWeights1_init, sizeof(uint8_t) * 16);
  static uint8_t recurrentToCellWeights1_init[] = {172, 60, 205, 65, 14, 0, 140, 168, 240, 223, 133, 56, 142, 64, 246, 216};
  model->setOperandValue(recurrentToCellWeights1, recurrentToCellWeights1_init, sizeof(uint8_t) * 16);
  static uint8_t recurrentToOutputWeights1_init[] = {106, 214, 67, 23, 59, 158, 45, 3, 119, 132, 49, 205, 129, 218, 11, 98};
  model->setOperandValue(recurrentToOutputWeights1, recurrentToOutputWeights1_init, sizeof(uint8_t) * 16);
  static int32_t inputGateBias1_init[] = {-7876, 13488, -726, 32839};
  model->setOperandValue(inputGateBias1, inputGateBias1_init, sizeof(int32_t) * 4);
  static int32_t forgetGateBias1_init[] = {9206, -46884, -11693, -38724};
  model->setOperandValue(forgetGateBias1, forgetGateBias1_init, sizeof(int32_t) * 4);
  static int32_t cellGateBias1_init[] = {39481, 48624, 48976, -21419};
  model->setOperandValue(cellGateBias1, cellGateBias1_init, sizeof(int32_t) * 4);
  static int32_t outputGateBias1_init[] = {-58999, -17050, -41852, -40538};
  model->setOperandValue(outputGateBias1, outputGateBias1_init, sizeof(int32_t) * 4);
  model->addOperation(ANEURALNETWORKS_QUANTIZED_16BIT_LSTM, {input1, inputToInputWeights1, inputToForgetWeights1, inputToCellWeights1, inputToOutputWeights1, recurrentToInputWeights1, recurrentToForgetWeights1, recurrentToCellWeights1, recurrentToOutputWeights1, inputGateBias1, forgetGateBias1, cellGateBias1, outputGateBias1, prevCellState1, prevOutput1}, {cellStateOut1, output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input1, prevCellState1, prevOutput1},
    {cellStateOut1, output1});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_2(Model *model) {
  OperandType type1(Type::TENSOR_QUANT8_ASYMM, {4, 2}, 0.00408021f, 100);
  OperandType type10(Type::TENSOR_QUANT8_ASYMM, {0, 0}, 0.0078125f, 128);
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {4, 4}, 0.00408021f, 100);
  OperandType type3(Type::TENSOR_INT32, {4}, 3.1876640625e-05f, 0);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.0078125f, 128);
  OperandType type7(Type::TENSOR_QUANT16_SYMM, {1, 4}, 0.00048828125f, 0);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1, 4}, 0.0078125f, 128);
  OperandType type9(Type::TENSOR_QUANT16_SYMM, {0, 0}, 0.00048828125f, 0);
  // Phase 1, operands
  auto input1 = model->addOperand(&type6);
  auto inputToInputWeights1 = model->addOperand(&type1);
  auto inputToForgetWeights1 = model->addOperand(&type1);
  auto inputToCellWeights1 = model->addOperand(&type1);
  auto inputToOutputWeights1 = model->addOperand(&type1);
  auto recurrentToInputWeights1 = model->addOperand(&type2);
  auto recurrentToForgetWeights1 = model->addOperand(&type2);
  auto recurrentToCellWeights1 = model->addOperand(&type2);
  auto recurrentToOutputWeights1 = model->addOperand(&type2);
  auto inputGateBias1 = model->addOperand(&type3);
  auto forgetGateBias1 = model->addOperand(&type3);
  auto cellGateBias1 = model->addOperand(&type3);
  auto outputGateBias1 = model->addOperand(&type3);
  auto prevCellState1 = model->addOperand(&type7);
  auto prevOutput1 = model->addOperand(&type8);
  auto cellStateOut1 = model->addOperand(&type9);
  auto output1 = model->addOperand(&type10);
  // Phase 2, operations
  static uint8_t inputToInputWeights1_init[] = {146, 250, 235, 171, 10, 218, 171, 108};
  model->setOperandValue(inputToInputWeights1, inputToInputWeights1_init, sizeof(uint8_t) * 8);
  static uint8_t inputToForgetWeights1_init[] = {24, 50, 132, 179, 158, 110, 3, 169};
  model->setOperandValue(inputToForgetWeights1, inputToForgetWeights1_init, sizeof(uint8_t) * 8);
  static uint8_t inputToCellWeights1_init[] = {133, 34, 29, 49, 206, 109, 54, 183};
  model->setOperandValue(inputToCellWeights1, inputToCellWeights1_init, sizeof(uint8_t) * 8);
  static uint8_t inputToOutputWeights1_init[] = {195, 187, 11, 99, 109, 10, 218, 48};
  model->setOperandValue(inputToOutputWeights1, inputToOutputWeights1_init, sizeof(uint8_t) * 8);
  static uint8_t recurrentToInputWeights1_init[] = {254, 206, 77, 168, 71, 20, 215, 6, 223, 7, 118, 225, 59, 130, 174, 26};
  model->setOperandValue(recurrentToInputWeights1, recurrentToInputWeights1_init, sizeof(uint8_t) * 16);
  static uint8_t recurrentToForgetWeights1_init[] = {137, 240, 103, 52, 68, 51, 237, 112, 0, 220, 89, 23, 69, 4, 207, 253};
  model->setOperandValue(recurrentToForgetWeights1, recurrentToForgetWeights1_init, sizeof(uint8_t) * 16);
  static uint8_t recurrentToCellWeights1_init[] = {172, 60, 205, 65, 14, 0, 140, 168, 240, 223, 133, 56, 142, 64, 246, 216};
  model->setOperandValue(recurrentToCellWeights1, recurrentToCellWeights1_init, sizeof(uint8_t) * 16);
  static uint8_t recurrentToOutputWeights1_init[] = {106, 214, 67, 23, 59, 158, 45, 3, 119, 132, 49, 205, 129, 218, 11, 98};
  model->setOperandValue(recurrentToOutputWeights1, recurrentToOutputWeights1_init, sizeof(uint8_t) * 16);
  static int32_t inputGateBias1_init[] = {-7876, 13488, -726, 32839};
  model->setOperandValue(inputGateBias1, inputGateBias1_init, sizeof(int32_t) * 4);
  static int32_t forgetGateBias1_init[] = {9206, -46884, -11693, -38724};
  model->setOperandValue(forgetGateBias1, forgetGateBias1_init, sizeof(int32_t) * 4);
  static int32_t cellGateBias1_init[] = {39481, 48624, 48976, -21419};
  model->setOperandValue(cellGateBias1, cellGateBias1_init, sizeof(int32_t) * 4);
  static int32_t outputGateBias1_init[] = {-58999, -17050, -41852, -40538};
  model->setOperandValue(outputGateBias1, outputGateBias1_init, sizeof(int32_t) * 4);
  model->addOperation(ANEURALNETWORKS_QUANTIZED_16BIT_LSTM, {input1, inputToInputWeights1, inputToForgetWeights1, inputToCellWeights1, inputToOutputWeights1, recurrentToInputWeights1, recurrentToForgetWeights1, recurrentToCellWeights1, recurrentToOutputWeights1, inputGateBias1, forgetGateBias1, cellGateBias1, outputGateBias1, prevCellState1, prevOutput1}, {cellStateOut1, output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input1, prevCellState1, prevOutput1},
    {cellStateOut1, output1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_2(Model *model) {
  OperandType type1(Type::TENSOR_QUANT8_ASYMM, {4, 2}, 0.00408021f, 100);
  OperandType type10(Type::TENSOR_QUANT8_ASYMM, {0, 0}, 0.0078125f, 128);
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {4, 4}, 0.00408021f, 100);
  OperandType type3(Type::TENSOR_INT32, {4}, 3.1876640625e-05f, 0);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.0078125f, 128);
  OperandType type7(Type::TENSOR_QUANT16_SYMM, {1, 4}, 0.00048828125f, 0);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1, 4}, 0.0078125f, 128);
  OperandType type9(Type::TENSOR_QUANT16_SYMM, {0, 0}, 0.00048828125f, 0);
  // Phase 1, operands
  auto input1 = model->addOperand(&type6);
  auto inputToInputWeights1 = model->addOperand(&type1);
  auto inputToForgetWeights1 = model->addOperand(&type1);
  auto inputToCellWeights1 = model->addOperand(&type1);
  auto inputToOutputWeights1 = model->addOperand(&type1);
  auto recurrentToInputWeights1 = model->addOperand(&type2);
  auto recurrentToForgetWeights1 = model->addOperand(&type2);
  auto recurrentToCellWeights1 = model->addOperand(&type2);
  auto recurrentToOutputWeights1 = model->addOperand(&type2);
  auto inputGateBias1 = model->addOperand(&type3);
  auto forgetGateBias1 = model->addOperand(&type3);
  auto cellGateBias1 = model->addOperand(&type3);
  auto outputGateBias1 = model->addOperand(&type3);
  auto prevCellState1 = model->addOperand(&type7);
  auto prevOutput1 = model->addOperand(&type8);
  auto cellStateOut1 = model->addOperand(&type9);
  auto output1 = model->addOperand(&type10);
  // Phase 2, operations
  static uint8_t inputToInputWeights1_init[] = {146, 250, 235, 171, 10, 218, 171, 108};
  model->setOperandValue(inputToInputWeights1, inputToInputWeights1_init, sizeof(uint8_t) * 8);
  static uint8_t inputToForgetWeights1_init[] = {24, 50, 132, 179, 158, 110, 3, 169};
  model->setOperandValue(inputToForgetWeights1, inputToForgetWeights1_init, sizeof(uint8_t) * 8);
  static uint8_t inputToCellWeights1_init[] = {133, 34, 29, 49, 206, 109, 54, 183};
  model->setOperandValue(inputToCellWeights1, inputToCellWeights1_init, sizeof(uint8_t) * 8);
  static uint8_t inputToOutputWeights1_init[] = {195, 187, 11, 99, 109, 10, 218, 48};
  model->setOperandValue(inputToOutputWeights1, inputToOutputWeights1_init, sizeof(uint8_t) * 8);
  static uint8_t recurrentToInputWeights1_init[] = {254, 206, 77, 168, 71, 20, 215, 6, 223, 7, 118, 225, 59, 130, 174, 26};
  model->setOperandValue(recurrentToInputWeights1, recurrentToInputWeights1_init, sizeof(uint8_t) * 16);
  static uint8_t recurrentToForgetWeights1_init[] = {137, 240, 103, 52, 68, 51, 237, 112, 0, 220, 89, 23, 69, 4, 207, 253};
  model->setOperandValue(recurrentToForgetWeights1, recurrentToForgetWeights1_init, sizeof(uint8_t) * 16);
  static uint8_t recurrentToCellWeights1_init[] = {172, 60, 205, 65, 14, 0, 140, 168, 240, 223, 133, 56, 142, 64, 246, 216};
  model->setOperandValue(recurrentToCellWeights1, recurrentToCellWeights1_init, sizeof(uint8_t) * 16);
  static uint8_t recurrentToOutputWeights1_init[] = {106, 214, 67, 23, 59, 158, 45, 3, 119, 132, 49, 205, 129, 218, 11, 98};
  model->setOperandValue(recurrentToOutputWeights1, recurrentToOutputWeights1_init, sizeof(uint8_t) * 16);
  static int32_t inputGateBias1_init[] = {-7876, 13488, -726, 32839};
  model->setOperandValue(inputGateBias1, inputGateBias1_init, sizeof(int32_t) * 4);
  static int32_t forgetGateBias1_init[] = {9206, -46884, -11693, -38724};
  model->setOperandValue(forgetGateBias1, forgetGateBias1_init, sizeof(int32_t) * 4);
  static int32_t cellGateBias1_init[] = {39481, 48624, 48976, -21419};
  model->setOperandValue(cellGateBias1, cellGateBias1_init, sizeof(int32_t) * 4);
  static int32_t outputGateBias1_init[] = {-58999, -17050, -41852, -40538};
  model->setOperandValue(outputGateBias1, outputGateBias1_init, sizeof(int32_t) * 4);
  model->addOperation(ANEURALNETWORKS_QUANTIZED_16BIT_LSTM, {input1, inputToInputWeights1, inputToForgetWeights1, inputToCellWeights1, inputToOutputWeights1, recurrentToInputWeights1, recurrentToForgetWeights1, recurrentToCellWeights1, recurrentToOutputWeights1, inputGateBias1, forgetGateBias1, cellGateBias1, outputGateBias1, prevCellState1, prevOutput1}, {cellStateOut1, output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input1, prevCellState1, prevOutput1},
    {cellStateOut1, output1});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

