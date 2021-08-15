// clang-format off
// Generated file (from: topk_v2.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto k = model->addOperand(&type1);
  auto out_values = model->addOperand(&type0);
  auto out_indices = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t k_init[] = {2};
  model->setOperandValue(k, k_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input, k}, {out_values, out_indices});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input},
    {out_values, out_indices});
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto k = model->addOperand(&type1);
  auto out_values = model->addOperand(&type0);
  auto out_indices = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t k_init[] = {2};
  model->setOperandValue(k, k_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input, k}, {out_values, out_indices});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input},
    {out_values, out_indices});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type11(Type::TENSOR_FLOAT16, {2, 2});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  // Phase 1, operands
  auto input = model->addOperand(&type11);
  auto k = model->addOperand(&type1);
  auto out_values = model->addOperand(&type11);
  auto out_indices = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t k_init[] = {2};
  model->setOperandValue(k, k_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input, k}, {out_values, out_indices});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input},
    {out_values, out_indices});
  assert(model->isValid());
}

inline bool is_ignored_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2});
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type13(Type::TENSOR_INT32, {0, 0});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto k = model->addOperand(&type1);
  auto out_values = model->addOperand(&type12);
  auto out_indices = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t k_init[] = {2};
  model->setOperandValue(k, k_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input, k}, {out_values, out_indices});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input},
    {out_values, out_indices});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2});
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type13(Type::TENSOR_INT32, {0, 0});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto k = model->addOperand(&type1);
  auto out_values = model->addOperand(&type12);
  auto out_indices = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t k_init[] = {2};
  model->setOperandValue(k, k_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input, k}, {out_values, out_indices});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input},
    {out_values, out_indices});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type11(Type::TENSOR_FLOAT16, {2, 2});
  OperandType type13(Type::TENSOR_INT32, {0, 0});
  OperandType type14(Type::TENSOR_FLOAT16, {0, 0});
  // Phase 1, operands
  auto input = model->addOperand(&type11);
  auto k = model->addOperand(&type1);
  auto out_values = model->addOperand(&type14);
  auto out_indices = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t k_init[] = {2};
  model->setOperandValue(k, k_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input, k}, {out_values, out_indices});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input},
    {out_values, out_indices});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {2, 3});
  // Phase 1, operands
  auto input1 = model->addOperand(&type3);
  auto k1 = model->addOperand(&type1);
  auto out_values1 = model->addOperand(&type0);
  auto out_indices1 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t k1_init[] = {2};
  model->setOperandValue(k1, k1_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input1, k1}, {out_values1, out_indices1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input1},
    {out_values1, out_indices1});
  assert(model->isValid());
}

inline bool is_ignored_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {2, 3});
  // Phase 1, operands
  auto input1 = model->addOperand(&type3);
  auto k1 = model->addOperand(&type1);
  auto out_values1 = model->addOperand(&type0);
  auto out_indices1 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t k1_init[] = {2};
  model->setOperandValue(k1, k1_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input1, k1}, {out_values1, out_indices1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input1},
    {out_values1, out_indices1});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type11(Type::TENSOR_FLOAT16, {2, 2});
  OperandType type15(Type::TENSOR_FLOAT16, {2, 3});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  // Phase 1, operands
  auto input1 = model->addOperand(&type15);
  auto k1 = model->addOperand(&type1);
  auto out_values1 = model->addOperand(&type11);
  auto out_indices1 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t k1_init[] = {2};
  model->setOperandValue(k1, k1_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input1, k1}, {out_values1, out_indices1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input1},
    {out_values1, out_indices1});
  assert(model->isValid());
}

inline bool is_ignored_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type13(Type::TENSOR_INT32, {0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {2, 3});
  // Phase 1, operands
  auto input1 = model->addOperand(&type3);
  auto k1 = model->addOperand(&type1);
  auto out_values1 = model->addOperand(&type12);
  auto out_indices1 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t k1_init[] = {2};
  model->setOperandValue(k1, k1_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input1, k1}, {out_values1, out_indices1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input1},
    {out_values1, out_indices1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type13(Type::TENSOR_INT32, {0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {2, 3});
  // Phase 1, operands
  auto input1 = model->addOperand(&type3);
  auto k1 = model->addOperand(&type1);
  auto out_values1 = model->addOperand(&type12);
  auto out_indices1 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t k1_init[] = {2};
  model->setOperandValue(k1, k1_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input1, k1}, {out_values1, out_indices1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input1},
    {out_values1, out_indices1});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_INT32, {0, 0});
  OperandType type14(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type15(Type::TENSOR_FLOAT16, {2, 3});
  // Phase 1, operands
  auto input1 = model->addOperand(&type15);
  auto k1 = model->addOperand(&type1);
  auto out_values1 = model->addOperand(&type14);
  auto out_indices1 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t k1_init[] = {2};
  model->setOperandValue(k1, k1_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input1, k1}, {out_values1, out_indices1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input1},
    {out_values1, out_indices1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 4});
  // Phase 1, operands
  auto input2 = model->addOperand(&type4);
  auto k2 = model->addOperand(&type1);
  auto out_values2 = model->addOperand(&type0);
  auto out_indices2 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t k2_init[] = {2};
  model->setOperandValue(k2, k2_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input2, k2}, {out_values2, out_indices2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input2},
    {out_values2, out_indices2});
  assert(model->isValid());
}

inline bool is_ignored_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_3(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 4});
  // Phase 1, operands
  auto input2 = model->addOperand(&type4);
  auto k2 = model->addOperand(&type1);
  auto out_values2 = model->addOperand(&type0);
  auto out_indices2 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t k2_init[] = {2};
  model->setOperandValue(k2, k2_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input2, k2}, {out_values2, out_indices2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input2},
    {out_values2, out_indices2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type11(Type::TENSOR_FLOAT16, {2, 2});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 4});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  // Phase 1, operands
  auto input2 = model->addOperand(&type16);
  auto k2 = model->addOperand(&type1);
  auto out_values2 = model->addOperand(&type11);
  auto out_indices2 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t k2_init[] = {2};
  model->setOperandValue(k2, k2_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input2, k2}, {out_values2, out_indices2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input2},
    {out_values2, out_indices2});
  assert(model->isValid());
}

inline bool is_ignored_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type13(Type::TENSOR_INT32, {0, 0});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 4});
  // Phase 1, operands
  auto input2 = model->addOperand(&type4);
  auto k2 = model->addOperand(&type1);
  auto out_values2 = model->addOperand(&type12);
  auto out_indices2 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t k2_init[] = {2};
  model->setOperandValue(k2, k2_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input2, k2}, {out_values2, out_indices2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input2},
    {out_values2, out_indices2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type13(Type::TENSOR_INT32, {0, 0});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 4});
  // Phase 1, operands
  auto input2 = model->addOperand(&type4);
  auto k2 = model->addOperand(&type1);
  auto out_values2 = model->addOperand(&type12);
  auto out_indices2 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t k2_init[] = {2};
  model->setOperandValue(k2, k2_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input2, k2}, {out_values2, out_indices2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input2},
    {out_values2, out_indices2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_INT32, {0, 0});
  OperandType type14(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 4});
  // Phase 1, operands
  auto input2 = model->addOperand(&type16);
  auto k2 = model->addOperand(&type1);
  auto out_values2 = model->addOperand(&type14);
  auto out_indices2 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t k2_init[] = {2};
  model->setOperandValue(k2, k2_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input2, k2}, {out_values2, out_indices2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input2},
    {out_values2, out_indices2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_4(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {8});
  OperandType type6(Type::TENSOR_FLOAT32, {2});
  OperandType type7(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto input3 = model->addOperand(&type5);
  auto k3 = model->addOperand(&type1);
  auto out_values3 = model->addOperand(&type6);
  auto out_indices3 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t k3_init[] = {2};
  model->setOperandValue(k3, k3_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input3, k3}, {out_values3, out_indices3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input3},
    {out_values3, out_indices3});
  assert(model->isValid());
}

inline bool is_ignored_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_4(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {8});
  OperandType type6(Type::TENSOR_FLOAT32, {2});
  OperandType type7(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto input3 = model->addOperand(&type5);
  auto k3 = model->addOperand(&type1);
  auto out_values3 = model->addOperand(&type6);
  auto out_indices3 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t k3_init[] = {2};
  model->setOperandValue(k3, k3_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input3, k3}, {out_values3, out_indices3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input3},
    {out_values3, out_indices3});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_4(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type17(Type::TENSOR_FLOAT16, {8});
  OperandType type18(Type::TENSOR_FLOAT16, {2});
  OperandType type7(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto input3 = model->addOperand(&type17);
  auto k3 = model->addOperand(&type1);
  auto out_values3 = model->addOperand(&type18);
  auto out_indices3 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t k3_init[] = {2};
  model->setOperandValue(k3, k3_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input3, k3}, {out_values3, out_indices3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input3},
    {out_values3, out_indices3});
  assert(model->isValid());
}

inline bool is_ignored_float16_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_4(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_FLOAT32, {0});
  OperandType type20(Type::TENSOR_INT32, {0});
  OperandType type5(Type::TENSOR_FLOAT32, {8});
  // Phase 1, operands
  auto input3 = model->addOperand(&type5);
  auto k3 = model->addOperand(&type1);
  auto out_values3 = model->addOperand(&type19);
  auto out_indices3 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t k3_init[] = {2};
  model->setOperandValue(k3, k3_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input3, k3}, {out_values3, out_indices3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input3},
    {out_values3, out_indices3});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_4(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_FLOAT32, {0});
  OperandType type20(Type::TENSOR_INT32, {0});
  OperandType type5(Type::TENSOR_FLOAT32, {8});
  // Phase 1, operands
  auto input3 = model->addOperand(&type5);
  auto k3 = model->addOperand(&type1);
  auto out_values3 = model->addOperand(&type19);
  auto out_indices3 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t k3_init[] = {2};
  model->setOperandValue(k3, k3_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input3, k3}, {out_values3, out_indices3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input3},
    {out_values3, out_indices3});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_4(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type17(Type::TENSOR_FLOAT16, {8});
  OperandType type20(Type::TENSOR_INT32, {0});
  OperandType type21(Type::TENSOR_FLOAT16, {0});
  // Phase 1, operands
  auto input3 = model->addOperand(&type17);
  auto k3 = model->addOperand(&type1);
  auto out_values3 = model->addOperand(&type21);
  auto out_indices3 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t k3_init[] = {2};
  model->setOperandValue(k3, k3_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input3, k3}, {out_values3, out_indices3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input3},
    {out_values3, out_indices3});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_5(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {2, 3}, 2.0f, 128);
  OperandType type9(Type::TENSOR_QUANT8_ASYMM, {2, 2}, 2.0f, 128);
  // Phase 1, operands
  auto input4 = model->addOperand(&type8);
  auto k4 = model->addOperand(&type1);
  auto out_values4 = model->addOperand(&type9);
  auto out_indices4 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t k4_init[] = {2};
  model->setOperandValue(k4, k4_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input4, k4}, {out_values4, out_indices4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input4},
    {out_values4, out_indices4});
  assert(model->isValid());
}

inline bool is_ignored_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_5(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {2, 3}, 2.0f, 128);
  OperandType type9(Type::TENSOR_QUANT8_ASYMM, {2, 2}, 2.0f, 128);
  // Phase 1, operands
  auto input4 = model->addOperand(&type8);
  auto k4 = model->addOperand(&type1);
  auto out_values4 = model->addOperand(&type9);
  auto out_indices4 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t k4_init[] = {2};
  model->setOperandValue(k4, k4_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input4, k4}, {out_values4, out_indices4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input4},
    {out_values4, out_indices4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_5(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {2, 3}, 2.0f, 128);
  OperandType type9(Type::TENSOR_QUANT8_ASYMM, {2, 2}, 2.0f, 128);
  // Phase 1, operands
  auto input4 = model->addOperand(&type8);
  auto k4 = model->addOperand(&type1);
  auto out_values4 = model->addOperand(&type9);
  auto out_indices4 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t k4_init[] = {2};
  model->setOperandValue(k4, k4_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input4, k4}, {out_values4, out_indices4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input4},
    {out_values4, out_indices4});
  assert(model->isValid());
}

inline bool is_ignored_float16_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_5(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_INT32, {0, 0});
  OperandType type22(Type::TENSOR_QUANT8_ASYMM, {0, 0}, 2.0f, 128);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {2, 3}, 2.0f, 128);
  // Phase 1, operands
  auto input4 = model->addOperand(&type8);
  auto k4 = model->addOperand(&type1);
  auto out_values4 = model->addOperand(&type22);
  auto out_indices4 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t k4_init[] = {2};
  model->setOperandValue(k4, k4_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input4, k4}, {out_values4, out_indices4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input4},
    {out_values4, out_indices4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_5(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_INT32, {0, 0});
  OperandType type22(Type::TENSOR_QUANT8_ASYMM, {0, 0}, 2.0f, 128);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {2, 3}, 2.0f, 128);
  // Phase 1, operands
  auto input4 = model->addOperand(&type8);
  auto k4 = model->addOperand(&type1);
  auto out_values4 = model->addOperand(&type22);
  auto out_indices4 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t k4_init[] = {2};
  model->setOperandValue(k4, k4_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input4, k4}, {out_values4, out_indices4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input4},
    {out_values4, out_indices4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_5(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_INT32, {0, 0});
  OperandType type22(Type::TENSOR_QUANT8_ASYMM, {0, 0}, 2.0f, 128);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {2, 3}, 2.0f, 128);
  // Phase 1, operands
  auto input4 = model->addOperand(&type8);
  auto k4 = model->addOperand(&type1);
  auto out_values4 = model->addOperand(&type22);
  auto out_indices4 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t k4_init[] = {2};
  model->setOperandValue(k4, k4_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input4, k4}, {out_values4, out_indices4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input4},
    {out_values4, out_indices4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_6(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::TENSOR_INT32, {2, 3});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  // Phase 1, operands
  auto input5 = model->addOperand(&type10);
  auto k5 = model->addOperand(&type1);
  auto out_values5 = model->addOperand(&type2);
  auto out_indices5 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t k5_init[] = {2};
  model->setOperandValue(k5, k5_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input5, k5}, {out_values5, out_indices5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input5},
    {out_values5, out_indices5});
  assert(model->isValid());
}

inline bool is_ignored_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_6(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::TENSOR_INT32, {2, 3});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  // Phase 1, operands
  auto input5 = model->addOperand(&type10);
  auto k5 = model->addOperand(&type1);
  auto out_values5 = model->addOperand(&type2);
  auto out_indices5 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t k5_init[] = {2};
  model->setOperandValue(k5, k5_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input5, k5}, {out_values5, out_indices5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input5},
    {out_values5, out_indices5});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_6(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::TENSOR_INT32, {2, 3});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  // Phase 1, operands
  auto input5 = model->addOperand(&type10);
  auto k5 = model->addOperand(&type1);
  auto out_values5 = model->addOperand(&type2);
  auto out_indices5 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t k5_init[] = {2};
  model->setOperandValue(k5, k5_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input5, k5}, {out_values5, out_indices5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input5},
    {out_values5, out_indices5});
  assert(model->isValid());
}

inline bool is_ignored_float16_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_6(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::TENSOR_INT32, {2, 3});
  OperandType type13(Type::TENSOR_INT32, {0, 0});
  // Phase 1, operands
  auto input5 = model->addOperand(&type10);
  auto k5 = model->addOperand(&type1);
  auto out_values5 = model->addOperand(&type13);
  auto out_indices5 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t k5_init[] = {2};
  model->setOperandValue(k5, k5_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input5, k5}, {out_values5, out_indices5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input5},
    {out_values5, out_indices5});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_6(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::TENSOR_INT32, {2, 3});
  OperandType type13(Type::TENSOR_INT32, {0, 0});
  // Phase 1, operands
  auto input5 = model->addOperand(&type10);
  auto k5 = model->addOperand(&type1);
  auto out_values5 = model->addOperand(&type13);
  auto out_indices5 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t k5_init[] = {2};
  model->setOperandValue(k5, k5_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input5, k5}, {out_values5, out_indices5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input5},
    {out_values5, out_indices5});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_6(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::TENSOR_INT32, {2, 3});
  OperandType type13(Type::TENSOR_INT32, {0, 0});
  // Phase 1, operands
  auto input5 = model->addOperand(&type10);
  auto k5 = model->addOperand(&type1);
  auto out_values5 = model->addOperand(&type13);
  auto out_indices5 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t k5_init[] = {2};
  model->setOperandValue(k5, k5_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_TOPK_V2, {input5, k5}, {out_values5, out_indices5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input5},
    {out_values5, out_indices5});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

