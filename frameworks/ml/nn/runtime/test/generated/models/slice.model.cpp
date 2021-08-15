// clang-format off
// Generated file (from: slice.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {4});
  OperandType type1(Type::TENSOR_INT32, {1});
  OperandType type2(Type::TENSOR_FLOAT32, {2});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto begin = model->addOperand(&type1);
  auto size = model->addOperand(&type1);
  auto output = model->addOperand(&type2);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input, begin, size}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, begin, size},
    {output});
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {4});
  OperandType type1(Type::TENSOR_INT32, {1});
  OperandType type2(Type::TENSOR_FLOAT32, {2});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto begin = model->addOperand(&type1);
  auto size = model->addOperand(&type1);
  auto output = model->addOperand(&type2);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input, begin, size}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, begin, size},
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
  OperandType type1(Type::TENSOR_INT32, {1});
  OperandType type28(Type::TENSOR_FLOAT16, {4});
  OperandType type29(Type::TENSOR_FLOAT16, {2});
  // Phase 1, operands
  auto input = model->addOperand(&type28);
  auto begin = model->addOperand(&type1);
  auto size = model->addOperand(&type1);
  auto output = model->addOperand(&type29);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input, begin, size}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, begin, size},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {4});
  OperandType type1(Type::TENSOR_INT32, {1});
  OperandType type17(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto begin = model->addOperand(&type1);
  auto size = model->addOperand(&type1);
  auto output = model->addOperand(&type17);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input, begin, size}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, begin, size},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {4});
  OperandType type1(Type::TENSOR_INT32, {1});
  OperandType type17(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto begin = model->addOperand(&type1);
  auto size = model->addOperand(&type1);
  auto output = model->addOperand(&type17);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input, begin, size}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, begin, size},
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
  OperandType type1(Type::TENSOR_INT32, {1});
  OperandType type28(Type::TENSOR_FLOAT16, {4});
  OperandType type30(Type::TENSOR_FLOAT16, {0});
  // Phase 1, operands
  auto input = model->addOperand(&type28);
  auto begin = model->addOperand(&type1);
  auto size = model->addOperand(&type1);
  auto output = model->addOperand(&type30);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input, begin, size}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, begin, size},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_2(Model *model) {
  OperandType type3(Type::TENSOR_FLOAT32, {2, 3});
  OperandType type4(Type::TENSOR_INT32, {2});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2});
  // Phase 1, operands
  auto input1 = model->addOperand(&type3);
  auto begin1 = model->addOperand(&type4);
  auto size1 = model->addOperand(&type4);
  auto output1 = model->addOperand(&type5);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input1, begin1, size1}, {output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input1, begin1, size1},
    {output1});
  assert(model->isValid());
}

inline bool is_ignored_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_2(Model *model) {
  OperandType type3(Type::TENSOR_FLOAT32, {2, 3});
  OperandType type4(Type::TENSOR_INT32, {2});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2});
  // Phase 1, operands
  auto input1 = model->addOperand(&type3);
  auto begin1 = model->addOperand(&type4);
  auto size1 = model->addOperand(&type4);
  auto output1 = model->addOperand(&type5);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input1, begin1, size1}, {output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input1, begin1, size1},
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
  OperandType type31(Type::TENSOR_FLOAT16, {2, 3});
  OperandType type32(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto input1 = model->addOperand(&type31);
  auto begin1 = model->addOperand(&type4);
  auto size1 = model->addOperand(&type4);
  auto output1 = model->addOperand(&type32);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input1, begin1, size1}, {output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input1, begin1, size1},
    {output1});
  assert(model->isValid());
}

inline bool is_ignored_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_2(Model *model) {
  OperandType type3(Type::TENSOR_FLOAT32, {2, 3});
  OperandType type33(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto input1 = model->addOperand(&type3);
  auto begin1 = model->addOperand(&type4);
  auto size1 = model->addOperand(&type4);
  auto output1 = model->addOperand(&type33);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input1, begin1, size1}, {output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input1, begin1, size1},
    {output1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_2(Model *model) {
  OperandType type3(Type::TENSOR_FLOAT32, {2, 3});
  OperandType type33(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto input1 = model->addOperand(&type3);
  auto begin1 = model->addOperand(&type4);
  auto size1 = model->addOperand(&type4);
  auto output1 = model->addOperand(&type33);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input1, begin1, size1}, {output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input1, begin1, size1},
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
  OperandType type31(Type::TENSOR_FLOAT16, {2, 3});
  OperandType type34(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto input1 = model->addOperand(&type31);
  auto begin1 = model->addOperand(&type4);
  auto size1 = model->addOperand(&type4);
  auto output1 = model->addOperand(&type34);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input1, begin1, size1}, {output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input1, begin1, size1},
    {output1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3(Model *model) {
  OperandType type6(Type::TENSOR_FLOAT32, {2, 3, 2});
  OperandType type7(Type::TENSOR_INT32, {3});
  // Phase 1, operands
  auto input2 = model->addOperand(&type6);
  auto begin2 = model->addOperand(&type7);
  auto size2 = model->addOperand(&type7);
  auto output2 = model->addOperand(&type6);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input2, begin2, size2}, {output2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input2, begin2, size2},
    {output2});
  assert(model->isValid());
}

inline bool is_ignored_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_3(Model *model) {
  OperandType type6(Type::TENSOR_FLOAT32, {2, 3, 2});
  OperandType type7(Type::TENSOR_INT32, {3});
  // Phase 1, operands
  auto input2 = model->addOperand(&type6);
  auto begin2 = model->addOperand(&type7);
  auto size2 = model->addOperand(&type7);
  auto output2 = model->addOperand(&type6);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input2, begin2, size2}, {output2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input2, begin2, size2},
    {output2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_3(Model *model) {
  OperandType type35(Type::TENSOR_FLOAT16, {2, 3, 2});
  OperandType type7(Type::TENSOR_INT32, {3});
  // Phase 1, operands
  auto input2 = model->addOperand(&type35);
  auto begin2 = model->addOperand(&type7);
  auto size2 = model->addOperand(&type7);
  auto output2 = model->addOperand(&type35);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input2, begin2, size2}, {output2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input2, begin2, size2},
    {output2});
  assert(model->isValid());
}

inline bool is_ignored_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_3(Model *model) {
  OperandType type36(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 3, 2});
  OperandType type7(Type::TENSOR_INT32, {3});
  // Phase 1, operands
  auto input2 = model->addOperand(&type6);
  auto begin2 = model->addOperand(&type7);
  auto size2 = model->addOperand(&type7);
  auto output2 = model->addOperand(&type36);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input2, begin2, size2}, {output2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input2, begin2, size2},
    {output2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_3(Model *model) {
  OperandType type36(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 3, 2});
  OperandType type7(Type::TENSOR_INT32, {3});
  // Phase 1, operands
  auto input2 = model->addOperand(&type6);
  auto begin2 = model->addOperand(&type7);
  auto size2 = model->addOperand(&type7);
  auto output2 = model->addOperand(&type36);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input2, begin2, size2}, {output2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input2, begin2, size2},
    {output2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_3(Model *model) {
  OperandType type35(Type::TENSOR_FLOAT16, {2, 3, 2});
  OperandType type37(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type7(Type::TENSOR_INT32, {3});
  // Phase 1, operands
  auto input2 = model->addOperand(&type35);
  auto begin2 = model->addOperand(&type7);
  auto size2 = model->addOperand(&type7);
  auto output2 = model->addOperand(&type37);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input2, begin2, size2}, {output2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input2, begin2, size2},
    {output2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_4(Model *model) {
  OperandType type10(Type::TENSOR_FLOAT32, {3, 1, 1, 1});
  OperandType type8(Type::TENSOR_FLOAT32, {4, 1, 1, 1});
  OperandType type9(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input3 = model->addOperand(&type8);
  auto begin3 = model->addOperand(&type9);
  auto size3 = model->addOperand(&type9);
  auto output3 = model->addOperand(&type10);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input3, begin3, size3}, {output3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input3, begin3, size3},
    {output3});
  assert(model->isValid());
}

inline bool is_ignored_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_4(Model *model) {
  OperandType type10(Type::TENSOR_FLOAT32, {3, 1, 1, 1});
  OperandType type8(Type::TENSOR_FLOAT32, {4, 1, 1, 1});
  OperandType type9(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input3 = model->addOperand(&type8);
  auto begin3 = model->addOperand(&type9);
  auto size3 = model->addOperand(&type9);
  auto output3 = model->addOperand(&type10);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input3, begin3, size3}, {output3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input3, begin3, size3},
    {output3});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_4(Model *model) {
  OperandType type38(Type::TENSOR_FLOAT16, {4, 1, 1, 1});
  OperandType type39(Type::TENSOR_FLOAT16, {3, 1, 1, 1});
  OperandType type9(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input3 = model->addOperand(&type38);
  auto begin3 = model->addOperand(&type9);
  auto size3 = model->addOperand(&type9);
  auto output3 = model->addOperand(&type39);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input3, begin3, size3}, {output3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input3, begin3, size3},
    {output3});
  assert(model->isValid());
}

inline bool is_ignored_float16_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_4(Model *model) {
  OperandType type40(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type8(Type::TENSOR_FLOAT32, {4, 1, 1, 1});
  OperandType type9(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input3 = model->addOperand(&type8);
  auto begin3 = model->addOperand(&type9);
  auto size3 = model->addOperand(&type9);
  auto output3 = model->addOperand(&type40);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input3, begin3, size3}, {output3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input3, begin3, size3},
    {output3});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_4(Model *model) {
  OperandType type40(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type8(Type::TENSOR_FLOAT32, {4, 1, 1, 1});
  OperandType type9(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input3 = model->addOperand(&type8);
  auto begin3 = model->addOperand(&type9);
  auto size3 = model->addOperand(&type9);
  auto output3 = model->addOperand(&type40);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input3, begin3, size3}, {output3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input3, begin3, size3},
    {output3});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_4(Model *model) {
  OperandType type38(Type::TENSOR_FLOAT16, {4, 1, 1, 1});
  OperandType type41(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type9(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input3 = model->addOperand(&type38);
  auto begin3 = model->addOperand(&type9);
  auto size3 = model->addOperand(&type9);
  auto output3 = model->addOperand(&type41);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input3, begin3, size3}, {output3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input3, begin3, size3},
    {output3});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_5(Model *model) {
  OperandType type11(Type::TENSOR_INT32, {3, 2, 3, 1});
  OperandType type12(Type::TENSOR_INT32, {1, 1, 3, 1});
  OperandType type9(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input4 = model->addOperand(&type11);
  auto begin4 = model->addOperand(&type9);
  auto size4 = model->addOperand(&type9);
  auto output4 = model->addOperand(&type12);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input4, begin4, size4}, {output4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input4, begin4, size4},
    {output4});
  assert(model->isValid());
}

inline bool is_ignored_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_5(Model *model) {
  OperandType type11(Type::TENSOR_INT32, {3, 2, 3, 1});
  OperandType type12(Type::TENSOR_INT32, {1, 1, 3, 1});
  OperandType type9(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input4 = model->addOperand(&type11);
  auto begin4 = model->addOperand(&type9);
  auto size4 = model->addOperand(&type9);
  auto output4 = model->addOperand(&type12);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input4, begin4, size4}, {output4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input4, begin4, size4},
    {output4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_5(Model *model) {
  OperandType type11(Type::TENSOR_INT32, {3, 2, 3, 1});
  OperandType type12(Type::TENSOR_INT32, {1, 1, 3, 1});
  OperandType type9(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input4 = model->addOperand(&type11);
  auto begin4 = model->addOperand(&type9);
  auto size4 = model->addOperand(&type9);
  auto output4 = model->addOperand(&type12);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input4, begin4, size4}, {output4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input4, begin4, size4},
    {output4});
  assert(model->isValid());
}

inline bool is_ignored_float16_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_5(Model *model) {
  OperandType type11(Type::TENSOR_INT32, {3, 2, 3, 1});
  OperandType type42(Type::TENSOR_INT32, {0, 0, 0, 0});
  OperandType type9(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input4 = model->addOperand(&type11);
  auto begin4 = model->addOperand(&type9);
  auto size4 = model->addOperand(&type9);
  auto output4 = model->addOperand(&type42);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input4, begin4, size4}, {output4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input4, begin4, size4},
    {output4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_5(Model *model) {
  OperandType type11(Type::TENSOR_INT32, {3, 2, 3, 1});
  OperandType type42(Type::TENSOR_INT32, {0, 0, 0, 0});
  OperandType type9(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input4 = model->addOperand(&type11);
  auto begin4 = model->addOperand(&type9);
  auto size4 = model->addOperand(&type9);
  auto output4 = model->addOperand(&type42);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input4, begin4, size4}, {output4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input4, begin4, size4},
    {output4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_5(Model *model) {
  OperandType type11(Type::TENSOR_INT32, {3, 2, 3, 1});
  OperandType type42(Type::TENSOR_INT32, {0, 0, 0, 0});
  OperandType type9(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input4 = model->addOperand(&type11);
  auto begin4 = model->addOperand(&type9);
  auto size4 = model->addOperand(&type9);
  auto output4 = model->addOperand(&type42);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input4, begin4, size4}, {output4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input4, begin4, size4},
    {output4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_6(Model *model) {
  OperandType type11(Type::TENSOR_INT32, {3, 2, 3, 1});
  OperandType type13(Type::TENSOR_INT32, {2, 1, 3, 1});
  OperandType type9(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input5 = model->addOperand(&type11);
  auto begin5 = model->addOperand(&type9);
  auto size5 = model->addOperand(&type9);
  auto output5 = model->addOperand(&type13);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input5, begin5, size5}, {output5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input5, begin5, size5},
    {output5});
  assert(model->isValid());
}

inline bool is_ignored_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_6(Model *model) {
  OperandType type11(Type::TENSOR_INT32, {3, 2, 3, 1});
  OperandType type13(Type::TENSOR_INT32, {2, 1, 3, 1});
  OperandType type9(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input5 = model->addOperand(&type11);
  auto begin5 = model->addOperand(&type9);
  auto size5 = model->addOperand(&type9);
  auto output5 = model->addOperand(&type13);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input5, begin5, size5}, {output5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input5, begin5, size5},
    {output5});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_6(Model *model) {
  OperandType type11(Type::TENSOR_INT32, {3, 2, 3, 1});
  OperandType type13(Type::TENSOR_INT32, {2, 1, 3, 1});
  OperandType type9(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input5 = model->addOperand(&type11);
  auto begin5 = model->addOperand(&type9);
  auto size5 = model->addOperand(&type9);
  auto output5 = model->addOperand(&type13);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input5, begin5, size5}, {output5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input5, begin5, size5},
    {output5});
  assert(model->isValid());
}

inline bool is_ignored_float16_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_6(Model *model) {
  OperandType type11(Type::TENSOR_INT32, {3, 2, 3, 1});
  OperandType type42(Type::TENSOR_INT32, {0, 0, 0, 0});
  OperandType type9(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input5 = model->addOperand(&type11);
  auto begin5 = model->addOperand(&type9);
  auto size5 = model->addOperand(&type9);
  auto output5 = model->addOperand(&type42);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input5, begin5, size5}, {output5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input5, begin5, size5},
    {output5});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_6(Model *model) {
  OperandType type11(Type::TENSOR_INT32, {3, 2, 3, 1});
  OperandType type42(Type::TENSOR_INT32, {0, 0, 0, 0});
  OperandType type9(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input5 = model->addOperand(&type11);
  auto begin5 = model->addOperand(&type9);
  auto size5 = model->addOperand(&type9);
  auto output5 = model->addOperand(&type42);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input5, begin5, size5}, {output5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input5, begin5, size5},
    {output5});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_6(Model *model) {
  OperandType type11(Type::TENSOR_INT32, {3, 2, 3, 1});
  OperandType type42(Type::TENSOR_INT32, {0, 0, 0, 0});
  OperandType type9(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input5 = model->addOperand(&type11);
  auto begin5 = model->addOperand(&type9);
  auto size5 = model->addOperand(&type9);
  auto output5 = model->addOperand(&type42);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input5, begin5, size5}, {output5});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input5, begin5, size5},
    {output5});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_7(Model *model) {
  OperandType type14(Type::TENSOR_QUANT8_ASYMM, {3, 2, 3, 1}, 2.0f, 128);
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {2, 1, 3, 1}, 2.0f, 128);
  OperandType type9(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input6 = model->addOperand(&type14);
  auto begin6 = model->addOperand(&type9);
  auto size6 = model->addOperand(&type9);
  auto output6 = model->addOperand(&type15);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input6, begin6, size6}, {output6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input6, begin6, size6},
    {output6});
  assert(model->isValid());
}

inline bool is_ignored_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_7(Model *model) {
  OperandType type14(Type::TENSOR_QUANT8_ASYMM, {3, 2, 3, 1}, 2.0f, 128);
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {2, 1, 3, 1}, 2.0f, 128);
  OperandType type9(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input6 = model->addOperand(&type14);
  auto begin6 = model->addOperand(&type9);
  auto size6 = model->addOperand(&type9);
  auto output6 = model->addOperand(&type15);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input6, begin6, size6}, {output6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input6, begin6, size6},
    {output6});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_7(Model *model) {
  OperandType type14(Type::TENSOR_QUANT8_ASYMM, {3, 2, 3, 1}, 2.0f, 128);
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {2, 1, 3, 1}, 2.0f, 128);
  OperandType type9(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input6 = model->addOperand(&type14);
  auto begin6 = model->addOperand(&type9);
  auto size6 = model->addOperand(&type9);
  auto output6 = model->addOperand(&type15);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input6, begin6, size6}, {output6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input6, begin6, size6},
    {output6});
  assert(model->isValid());
}

inline bool is_ignored_float16_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_7(Model *model) {
  OperandType type14(Type::TENSOR_QUANT8_ASYMM, {3, 2, 3, 1}, 2.0f, 128);
  OperandType type43(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 2.0f, 128);
  OperandType type9(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input6 = model->addOperand(&type14);
  auto begin6 = model->addOperand(&type9);
  auto size6 = model->addOperand(&type9);
  auto output6 = model->addOperand(&type43);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input6, begin6, size6}, {output6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input6, begin6, size6},
    {output6});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_7(Model *model) {
  OperandType type14(Type::TENSOR_QUANT8_ASYMM, {3, 2, 3, 1}, 2.0f, 128);
  OperandType type43(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 2.0f, 128);
  OperandType type9(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input6 = model->addOperand(&type14);
  auto begin6 = model->addOperand(&type9);
  auto size6 = model->addOperand(&type9);
  auto output6 = model->addOperand(&type43);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input6, begin6, size6}, {output6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input6, begin6, size6},
    {output6});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_7(Model *model) {
  OperandType type14(Type::TENSOR_QUANT8_ASYMM, {3, 2, 3, 1}, 2.0f, 128);
  OperandType type43(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 2.0f, 128);
  OperandType type9(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input6 = model->addOperand(&type14);
  auto begin6 = model->addOperand(&type9);
  auto size6 = model->addOperand(&type9);
  auto output6 = model->addOperand(&type43);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input6, begin6, size6}, {output6});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input6, begin6, size6},
    {output6});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_8(Model *model) {
  OperandType type11(Type::TENSOR_INT32, {3, 2, 3, 1});
  OperandType type13(Type::TENSOR_INT32, {2, 1, 3, 1});
  OperandType type9(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input7 = model->addOperand(&type11);
  auto begin7 = model->addOperand(&type9);
  auto size7 = model->addOperand(&type9);
  auto output7 = model->addOperand(&type13);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input7, begin7, size7}, {output7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input7, begin7, size7},
    {output7});
  assert(model->isValid());
}

inline bool is_ignored_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_8(Model *model) {
  OperandType type11(Type::TENSOR_INT32, {3, 2, 3, 1});
  OperandType type13(Type::TENSOR_INT32, {2, 1, 3, 1});
  OperandType type9(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input7 = model->addOperand(&type11);
  auto begin7 = model->addOperand(&type9);
  auto size7 = model->addOperand(&type9);
  auto output7 = model->addOperand(&type13);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input7, begin7, size7}, {output7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input7, begin7, size7},
    {output7});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_8(Model *model) {
  OperandType type11(Type::TENSOR_INT32, {3, 2, 3, 1});
  OperandType type13(Type::TENSOR_INT32, {2, 1, 3, 1});
  OperandType type9(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input7 = model->addOperand(&type11);
  auto begin7 = model->addOperand(&type9);
  auto size7 = model->addOperand(&type9);
  auto output7 = model->addOperand(&type13);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input7, begin7, size7}, {output7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input7, begin7, size7},
    {output7});
  assert(model->isValid());
}

inline bool is_ignored_float16_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_8(Model *model) {
  OperandType type11(Type::TENSOR_INT32, {3, 2, 3, 1});
  OperandType type42(Type::TENSOR_INT32, {0, 0, 0, 0});
  OperandType type9(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input7 = model->addOperand(&type11);
  auto begin7 = model->addOperand(&type9);
  auto size7 = model->addOperand(&type9);
  auto output7 = model->addOperand(&type42);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input7, begin7, size7}, {output7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input7, begin7, size7},
    {output7});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_8(Model *model) {
  OperandType type11(Type::TENSOR_INT32, {3, 2, 3, 1});
  OperandType type42(Type::TENSOR_INT32, {0, 0, 0, 0});
  OperandType type9(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input7 = model->addOperand(&type11);
  auto begin7 = model->addOperand(&type9);
  auto size7 = model->addOperand(&type9);
  auto output7 = model->addOperand(&type42);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input7, begin7, size7}, {output7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input7, begin7, size7},
    {output7});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_8(Model *model) {
  OperandType type11(Type::TENSOR_INT32, {3, 2, 3, 1});
  OperandType type42(Type::TENSOR_INT32, {0, 0, 0, 0});
  OperandType type9(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input7 = model->addOperand(&type11);
  auto begin7 = model->addOperand(&type9);
  auto size7 = model->addOperand(&type9);
  auto output7 = model->addOperand(&type42);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SLICE, {input7, begin7, size7}, {output7});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input7, begin7, size7},
    {output7});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized(Model *model) {
  OperandType type16(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type17(Type::TENSOR_FLOAT32, {0});
  OperandType type18(Type::TENSOR_INT32, {0});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type20(Type::TENSOR_INT32, {1});
  OperandType type21(Type::FLOAT32, {});
  OperandType type22(Type::INT32, {});
  OperandType type23(Type::BOOL, {});
  OperandType type24(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 1, 1, 1});
  OperandType type27(Type::TENSOR_INT32, {4});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2});
  // Phase 1, operands
  auto scores = model->addOperand(&type5);
  auto roi = model->addOperand(&type16);
  auto param = model->addOperand(&type20);
  auto param1 = model->addOperand(&type21);
  auto param2 = model->addOperand(&type22);
  auto param3 = model->addOperand(&type22);
  auto param4 = model->addOperand(&type21);
  auto param5 = model->addOperand(&type21);
  auto param6 = model->addOperand(&type21);
  auto scoresOut = model->addOperand(&type17);
  auto roiOut = model->addOperand(&type19);
  auto classesOut = model->addOperand(&type18);
  auto batchSplitOut = model->addOperand(&type18);
  auto in = model->addOperand(&type24);
  auto param7 = model->addOperand(&type22);
  auto param8 = model->addOperand(&type22);
  auto param9 = model->addOperand(&type21);
  auto param10 = model->addOperand(&type21);
  auto param11 = model->addOperand(&type22);
  auto param12 = model->addOperand(&type22);
  auto layout = model->addOperand(&type23);
  auto featureMap = model->addOperand(&type25);
  auto param13 = model->addOperand(&type27);
  auto param14 = model->addOperand(&type27);
  auto out = model->addOperand(&type26);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.3f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static int32_t param2_init[] = {-1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static float param4_init[] = {0.4f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {1.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {0.3f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static int32_t param7_init[] = {2};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {2.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {2.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static int32_t param11_init[] = {4};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {4};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param13_init[] = {0, 1, 1, 0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 4);
  static int32_t param14_init[] = {-1, 1, -1, 1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 4);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param, param1, param2, param3, param4, param5, param6}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param7, param8, param9, param10, param11, param12, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_SLICE, {featureMap, param13, param14}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_relaxed(Model *model) {
  OperandType type16(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type17(Type::TENSOR_FLOAT32, {0});
  OperandType type18(Type::TENSOR_INT32, {0});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type20(Type::TENSOR_INT32, {1});
  OperandType type21(Type::FLOAT32, {});
  OperandType type22(Type::INT32, {});
  OperandType type23(Type::BOOL, {});
  OperandType type24(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 1, 1, 1});
  OperandType type27(Type::TENSOR_INT32, {4});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2});
  // Phase 1, operands
  auto scores = model->addOperand(&type5);
  auto roi = model->addOperand(&type16);
  auto param = model->addOperand(&type20);
  auto param1 = model->addOperand(&type21);
  auto param2 = model->addOperand(&type22);
  auto param3 = model->addOperand(&type22);
  auto param4 = model->addOperand(&type21);
  auto param5 = model->addOperand(&type21);
  auto param6 = model->addOperand(&type21);
  auto scoresOut = model->addOperand(&type17);
  auto roiOut = model->addOperand(&type19);
  auto classesOut = model->addOperand(&type18);
  auto batchSplitOut = model->addOperand(&type18);
  auto in = model->addOperand(&type24);
  auto param7 = model->addOperand(&type22);
  auto param8 = model->addOperand(&type22);
  auto param9 = model->addOperand(&type21);
  auto param10 = model->addOperand(&type21);
  auto param11 = model->addOperand(&type22);
  auto param12 = model->addOperand(&type22);
  auto layout = model->addOperand(&type23);
  auto featureMap = model->addOperand(&type25);
  auto param13 = model->addOperand(&type27);
  auto param14 = model->addOperand(&type27);
  auto out = model->addOperand(&type26);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.3f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static int32_t param2_init[] = {-1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static float param4_init[] = {0.4f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {1.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {0.3f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static int32_t param7_init[] = {2};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {2.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {2.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static int32_t param11_init[] = {4};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {4};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param13_init[] = {0, 1, 1, 0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 4);
  static int32_t param14_init[] = {-1, 1, -1, 1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 4);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param, param1, param2, param3, param4, param5, param6}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param7, param8, param9, param10, param11, param12, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_SLICE, {featureMap, param13, param14}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_quant8(Model *model) {
  OperandType type18(Type::TENSOR_INT32, {0});
  OperandType type20(Type::TENSOR_INT32, {1});
  OperandType type21(Type::FLOAT32, {});
  OperandType type22(Type::INT32, {});
  OperandType type23(Type::BOOL, {});
  OperandType type27(Type::TENSOR_INT32, {4});
  OperandType type44(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 1}, 0.1f, 128);
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.1f, 128);
  OperandType type46(Type::TENSOR_QUANT8_ASYMM, {0, 1, 1, 1}, 0.1f, 128);
  OperandType type47(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type48(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type49(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type50(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  // Phase 1, operands
  auto scores = model->addOperand(&type49);
  auto roi = model->addOperand(&type47);
  auto param = model->addOperand(&type20);
  auto param1 = model->addOperand(&type21);
  auto param2 = model->addOperand(&type22);
  auto param3 = model->addOperand(&type22);
  auto param4 = model->addOperand(&type21);
  auto param5 = model->addOperand(&type21);
  auto param6 = model->addOperand(&type21);
  auto scoresOut = model->addOperand(&type50);
  auto roiOut = model->addOperand(&type48);
  auto classesOut = model->addOperand(&type18);
  auto batchSplitOut = model->addOperand(&type18);
  auto in = model->addOperand(&type45);
  auto param7 = model->addOperand(&type22);
  auto param8 = model->addOperand(&type22);
  auto param9 = model->addOperand(&type21);
  auto param10 = model->addOperand(&type21);
  auto param11 = model->addOperand(&type22);
  auto param12 = model->addOperand(&type22);
  auto layout = model->addOperand(&type23);
  auto featureMap = model->addOperand(&type44);
  auto param13 = model->addOperand(&type27);
  auto param14 = model->addOperand(&type27);
  auto out = model->addOperand(&type46);
  // Phase 2, operations
  static uint8_t scores_init[] = {137, 129};
  model->setOperandValue(scores, scores_init, sizeof(uint8_t) * 2);
  static uint16_t roi_init[] = {8, 8, 80, 80, 0, 0, 80, 80};
  model->setOperandValue(roi, roi_init, sizeof(uint16_t) * 8);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.3f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static int32_t param2_init[] = {-1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static float param4_init[] = {0.4f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {1.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {0.3f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static int32_t param7_init[] = {2};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {2.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {2.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static int32_t param11_init[] = {4};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {4};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param13_init[] = {0, 1, 1, 0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 4);
  static int32_t param14_init[] = {-1, 1, -1, 1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 4);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param, param1, param2, param3, param4, param5, param6}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param7, param8, param9, param10, param11, param12, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_SLICE, {featureMap, param13, param14}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_float16(Model *model) {
  OperandType type18(Type::TENSOR_INT32, {0});
  OperandType type20(Type::TENSOR_INT32, {1});
  OperandType type22(Type::INT32, {});
  OperandType type23(Type::BOOL, {});
  OperandType type27(Type::TENSOR_INT32, {4});
  OperandType type32(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type51(Type::TENSOR_FLOAT16, {0, 2, 2, 1});
  OperandType type52(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type53(Type::TENSOR_FLOAT16, {0, 1, 1, 1});
  OperandType type54(Type::FLOAT16, {});
  OperandType type55(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type56(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type57(Type::TENSOR_FLOAT16, {0});
  // Phase 1, operands
  auto scores = model->addOperand(&type32);
  auto roi = model->addOperand(&type55);
  auto param = model->addOperand(&type20);
  auto param1 = model->addOperand(&type54);
  auto param2 = model->addOperand(&type22);
  auto param3 = model->addOperand(&type22);
  auto param4 = model->addOperand(&type54);
  auto param5 = model->addOperand(&type54);
  auto param6 = model->addOperand(&type54);
  auto scoresOut = model->addOperand(&type57);
  auto roiOut = model->addOperand(&type56);
  auto classesOut = model->addOperand(&type18);
  auto batchSplitOut = model->addOperand(&type18);
  auto in = model->addOperand(&type52);
  auto param7 = model->addOperand(&type22);
  auto param8 = model->addOperand(&type22);
  auto param9 = model->addOperand(&type54);
  auto param10 = model->addOperand(&type54);
  auto param11 = model->addOperand(&type22);
  auto param12 = model->addOperand(&type22);
  auto layout = model->addOperand(&type23);
  auto featureMap = model->addOperand(&type51);
  auto param13 = model->addOperand(&type27);
  auto param14 = model->addOperand(&type27);
  auto out = model->addOperand(&type53);
  // Phase 2, operations
  static _Float16 scores_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores, scores_init, sizeof(_Float16) * 2);
  static _Float16 roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(_Float16) * 8);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.30000001192092896f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static int32_t param2_init[] = {-1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static _Float16 param4_init[] = {0.4000000059604645f};
  model->setOperandValue(param4, param4_init, sizeof(_Float16) * 1);
  static _Float16 param5_init[] = {1.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {0.30000001192092896f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static int32_t param7_init[] = {2};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {2.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {2.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static int32_t param11_init[] = {4};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {4};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param13_init[] = {0, 1, 1, 0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 4);
  static int32_t param14_init[] = {-1, 1, -1, 1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 4);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param, param1, param2, param3, param4, param5, param6}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param7, param8, param9, param10, param11, param12, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_SLICE, {featureMap, param13, param14}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape(Model *model) {
  OperandType type16(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type17(Type::TENSOR_FLOAT32, {0});
  OperandType type18(Type::TENSOR_INT32, {0});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type20(Type::TENSOR_INT32, {1});
  OperandType type21(Type::FLOAT32, {});
  OperandType type22(Type::INT32, {});
  OperandType type23(Type::BOOL, {});
  OperandType type24(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type27(Type::TENSOR_INT32, {4});
  OperandType type40(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2});
  // Phase 1, operands
  auto scores = model->addOperand(&type5);
  auto roi = model->addOperand(&type16);
  auto param = model->addOperand(&type20);
  auto param1 = model->addOperand(&type21);
  auto param2 = model->addOperand(&type22);
  auto param3 = model->addOperand(&type22);
  auto param4 = model->addOperand(&type21);
  auto param5 = model->addOperand(&type21);
  auto param6 = model->addOperand(&type21);
  auto scoresOut = model->addOperand(&type17);
  auto roiOut = model->addOperand(&type19);
  auto classesOut = model->addOperand(&type18);
  auto batchSplitOut = model->addOperand(&type18);
  auto in = model->addOperand(&type24);
  auto param7 = model->addOperand(&type22);
  auto param8 = model->addOperand(&type22);
  auto param9 = model->addOperand(&type21);
  auto param10 = model->addOperand(&type21);
  auto param11 = model->addOperand(&type22);
  auto param12 = model->addOperand(&type22);
  auto layout = model->addOperand(&type23);
  auto featureMap = model->addOperand(&type25);
  auto param13 = model->addOperand(&type27);
  auto param14 = model->addOperand(&type27);
  auto out = model->addOperand(&type40);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.3f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static int32_t param2_init[] = {-1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static float param4_init[] = {0.4f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {1.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {0.3f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static int32_t param7_init[] = {2};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {2.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {2.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static int32_t param11_init[] = {4};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {4};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param13_init[] = {0, 1, 1, 0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 4);
  static int32_t param14_init[] = {-1, 1, -1, 1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 4);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param, param1, param2, param3, param4, param5, param6}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param7, param8, param9, param10, param11, param12, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_SLICE, {featureMap, param13, param14}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_relaxed(Model *model) {
  OperandType type16(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type17(Type::TENSOR_FLOAT32, {0});
  OperandType type18(Type::TENSOR_INT32, {0});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type20(Type::TENSOR_INT32, {1});
  OperandType type21(Type::FLOAT32, {});
  OperandType type22(Type::INT32, {});
  OperandType type23(Type::BOOL, {});
  OperandType type24(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type27(Type::TENSOR_INT32, {4});
  OperandType type40(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2});
  // Phase 1, operands
  auto scores = model->addOperand(&type5);
  auto roi = model->addOperand(&type16);
  auto param = model->addOperand(&type20);
  auto param1 = model->addOperand(&type21);
  auto param2 = model->addOperand(&type22);
  auto param3 = model->addOperand(&type22);
  auto param4 = model->addOperand(&type21);
  auto param5 = model->addOperand(&type21);
  auto param6 = model->addOperand(&type21);
  auto scoresOut = model->addOperand(&type17);
  auto roiOut = model->addOperand(&type19);
  auto classesOut = model->addOperand(&type18);
  auto batchSplitOut = model->addOperand(&type18);
  auto in = model->addOperand(&type24);
  auto param7 = model->addOperand(&type22);
  auto param8 = model->addOperand(&type22);
  auto param9 = model->addOperand(&type21);
  auto param10 = model->addOperand(&type21);
  auto param11 = model->addOperand(&type22);
  auto param12 = model->addOperand(&type22);
  auto layout = model->addOperand(&type23);
  auto featureMap = model->addOperand(&type25);
  auto param13 = model->addOperand(&type27);
  auto param14 = model->addOperand(&type27);
  auto out = model->addOperand(&type40);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.3f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static int32_t param2_init[] = {-1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static float param4_init[] = {0.4f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {1.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {0.3f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static int32_t param7_init[] = {2};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {2.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {2.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static int32_t param11_init[] = {4};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {4};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param13_init[] = {0, 1, 1, 0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 4);
  static int32_t param14_init[] = {-1, 1, -1, 1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 4);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param, param1, param2, param3, param4, param5, param6}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param7, param8, param9, param10, param11, param12, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_SLICE, {featureMap, param13, param14}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_quant8(Model *model) {
  OperandType type18(Type::TENSOR_INT32, {0});
  OperandType type20(Type::TENSOR_INT32, {1});
  OperandType type21(Type::FLOAT32, {});
  OperandType type22(Type::INT32, {});
  OperandType type23(Type::BOOL, {});
  OperandType type27(Type::TENSOR_INT32, {4});
  OperandType type44(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 1}, 0.1f, 128);
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.1f, 128);
  OperandType type47(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type48(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type49(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type50(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  OperandType type58(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 128);
  // Phase 1, operands
  auto scores = model->addOperand(&type49);
  auto roi = model->addOperand(&type47);
  auto param = model->addOperand(&type20);
  auto param1 = model->addOperand(&type21);
  auto param2 = model->addOperand(&type22);
  auto param3 = model->addOperand(&type22);
  auto param4 = model->addOperand(&type21);
  auto param5 = model->addOperand(&type21);
  auto param6 = model->addOperand(&type21);
  auto scoresOut = model->addOperand(&type50);
  auto roiOut = model->addOperand(&type48);
  auto classesOut = model->addOperand(&type18);
  auto batchSplitOut = model->addOperand(&type18);
  auto in = model->addOperand(&type45);
  auto param7 = model->addOperand(&type22);
  auto param8 = model->addOperand(&type22);
  auto param9 = model->addOperand(&type21);
  auto param10 = model->addOperand(&type21);
  auto param11 = model->addOperand(&type22);
  auto param12 = model->addOperand(&type22);
  auto layout = model->addOperand(&type23);
  auto featureMap = model->addOperand(&type44);
  auto param13 = model->addOperand(&type27);
  auto param14 = model->addOperand(&type27);
  auto out = model->addOperand(&type58);
  // Phase 2, operations
  static uint8_t scores_init[] = {137, 129};
  model->setOperandValue(scores, scores_init, sizeof(uint8_t) * 2);
  static uint16_t roi_init[] = {8, 8, 80, 80, 0, 0, 80, 80};
  model->setOperandValue(roi, roi_init, sizeof(uint16_t) * 8);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.3f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static int32_t param2_init[] = {-1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static float param4_init[] = {0.4f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static float param5_init[] = {1.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {0.3f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static int32_t param7_init[] = {2};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {2.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {2.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static int32_t param11_init[] = {4};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {4};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param13_init[] = {0, 1, 1, 0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 4);
  static int32_t param14_init[] = {-1, 1, -1, 1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 4);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param, param1, param2, param3, param4, param5, param6}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param7, param8, param9, param10, param11, param12, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_SLICE, {featureMap, param13, param14}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_float16(Model *model) {
  OperandType type18(Type::TENSOR_INT32, {0});
  OperandType type20(Type::TENSOR_INT32, {1});
  OperandType type22(Type::INT32, {});
  OperandType type23(Type::BOOL, {});
  OperandType type27(Type::TENSOR_INT32, {4});
  OperandType type30(Type::TENSOR_FLOAT16, {0});
  OperandType type32(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type41(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type51(Type::TENSOR_FLOAT16, {0, 2, 2, 1});
  OperandType type52(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type54(Type::FLOAT16, {});
  OperandType type55(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type56(Type::TENSOR_FLOAT16, {0, 4});
  // Phase 1, operands
  auto scores = model->addOperand(&type32);
  auto roi = model->addOperand(&type55);
  auto param = model->addOperand(&type20);
  auto param1 = model->addOperand(&type54);
  auto param2 = model->addOperand(&type22);
  auto param3 = model->addOperand(&type22);
  auto param4 = model->addOperand(&type54);
  auto param5 = model->addOperand(&type54);
  auto param6 = model->addOperand(&type54);
  auto scoresOut = model->addOperand(&type30);
  auto roiOut = model->addOperand(&type56);
  auto classesOut = model->addOperand(&type18);
  auto batchSplitOut = model->addOperand(&type18);
  auto in = model->addOperand(&type52);
  auto param7 = model->addOperand(&type22);
  auto param8 = model->addOperand(&type22);
  auto param9 = model->addOperand(&type54);
  auto param10 = model->addOperand(&type54);
  auto param11 = model->addOperand(&type22);
  auto param12 = model->addOperand(&type22);
  auto layout = model->addOperand(&type23);
  auto featureMap = model->addOperand(&type51);
  auto param13 = model->addOperand(&type27);
  auto param14 = model->addOperand(&type27);
  auto out = model->addOperand(&type41);
  // Phase 2, operations
  static _Float16 scores_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores, scores_init, sizeof(_Float16) * 2);
  static _Float16 roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(_Float16) * 8);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.30000001192092896f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static int32_t param2_init[] = {-1};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static _Float16 param4_init[] = {0.4000000059604645f};
  model->setOperandValue(param4, param4_init, sizeof(_Float16) * 1);
  static _Float16 param5_init[] = {1.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {0.30000001192092896f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static int32_t param7_init[] = {2};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {2.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {2.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static int32_t param11_init[] = {4};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {4};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param13_init[] = {0, 1, 1, 0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 4);
  static int32_t param14_init[] = {-1, 1, -1, 1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 4);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param, param1, param2, param3, param4, param5, param6}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param7, param8, param9, param10, param11, param12, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_SLICE, {featureMap, param13, param14}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

