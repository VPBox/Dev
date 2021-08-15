// clang-format off
// Generated file (from: select_v1_2.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_BOOL8, {3});
  OperandType type1(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto input1 = model->addOperand(&type1);
  auto input2 = model->addOperand(&type1);
  auto output0 = model->addOperand(&type1);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SELECT, {input0, input1, input2}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1, input2},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_int32(Model *model) {
  OperandType type0(Type::TENSOR_BOOL8, {3});
  OperandType type6(Type::TENSOR_INT32, {3});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto input1 = model->addOperand(&type6);
  auto input2 = model->addOperand(&type6);
  auto output0 = model->addOperand(&type6);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SELECT, {input0, input1, input2}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1, input2},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_int32(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16(Model *model) {
  OperandType type0(Type::TENSOR_BOOL8, {3});
  OperandType type7(Type::TENSOR_FLOAT16, {3});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto input1 = model->addOperand(&type7);
  auto input2 = model->addOperand(&type7);
  auto output0 = model->addOperand(&type7);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SELECT, {input0, input1, input2}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1, input2},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_BOOL8, {3});
  OperandType type1(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto input1 = model->addOperand(&type1);
  auto input2 = model->addOperand(&type1);
  auto output0 = model->addOperand(&type1);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SELECT, {input0, input1, input2}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1, input2},
    {output0});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant8(Model *model) {
  OperandType type0(Type::TENSOR_BOOL8, {3});
  OperandType type10(Type::TENSOR_QUANT8_ASYMM, {3}, 1.0f, 128);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {3}, 1.5f, 129);
  OperandType type9(Type::TENSOR_QUANT8_ASYMM, {3}, 0.5f, 127);
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto input1 = model->addOperand(&type8);
  auto input2 = model->addOperand(&type9);
  auto output0 = model->addOperand(&type10);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SELECT, {input0, input1, input2}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1, input2},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_BOOL8, {3});
  OperandType type1(Type::TENSOR_FLOAT32, {3});
  OperandType type11(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto input1 = model->addOperand(&type1);
  auto input2 = model->addOperand(&type1);
  auto output0 = model->addOperand(&type11);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SELECT, {input0, input1, input2}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1, input2},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_int32(Model *model) {
  OperandType type0(Type::TENSOR_BOOL8, {3});
  OperandType type12(Type::TENSOR_INT32, {0});
  OperandType type6(Type::TENSOR_INT32, {3});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto input1 = model->addOperand(&type6);
  auto input2 = model->addOperand(&type6);
  auto output0 = model->addOperand(&type12);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SELECT, {input0, input1, input2}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1, input2},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_int32(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16(Model *model) {
  OperandType type0(Type::TENSOR_BOOL8, {3});
  OperandType type13(Type::TENSOR_FLOAT16, {0});
  OperandType type7(Type::TENSOR_FLOAT16, {3});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto input1 = model->addOperand(&type7);
  auto input2 = model->addOperand(&type7);
  auto output0 = model->addOperand(&type13);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SELECT, {input0, input1, input2}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1, input2},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_BOOL8, {3});
  OperandType type1(Type::TENSOR_FLOAT32, {3});
  OperandType type11(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto input1 = model->addOperand(&type1);
  auto input2 = model->addOperand(&type1);
  auto output0 = model->addOperand(&type11);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SELECT, {input0, input1, input2}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1, input2},
    {output0});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_quant8(Model *model) {
  OperandType type0(Type::TENSOR_BOOL8, {3});
  OperandType type14(Type::TENSOR_QUANT8_ASYMM, {0}, 1.0f, 128);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {3}, 1.5f, 129);
  OperandType type9(Type::TENSOR_QUANT8_ASYMM, {3}, 0.5f, 127);
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto input1 = model->addOperand(&type8);
  auto input2 = model->addOperand(&type9);
  auto output0 = model->addOperand(&type14);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SELECT, {input0, input1, input2}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1, input2},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_2(Model *model) {
  OperandType type2(Type::TENSOR_BOOL8, {2, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {2, 2});
  // Phase 1, operands
  auto input01 = model->addOperand(&type2);
  auto input11 = model->addOperand(&type3);
  auto input21 = model->addOperand(&type3);
  auto output01 = model->addOperand(&type3);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SELECT, {input01, input11, input21}, {output01});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input01, input11, input21},
    {output01});
  assert(model->isValid());
}

inline bool is_ignored_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_int32_2(Model *model) {
  OperandType type15(Type::TENSOR_INT32, {2, 2});
  OperandType type2(Type::TENSOR_BOOL8, {2, 2});
  // Phase 1, operands
  auto input01 = model->addOperand(&type2);
  auto input11 = model->addOperand(&type15);
  auto input21 = model->addOperand(&type15);
  auto output01 = model->addOperand(&type15);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SELECT, {input01, input11, input21}, {output01});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input01, input11, input21},
    {output01});
  assert(model->isValid());
}

inline bool is_ignored_int32_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_2(Model *model) {
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2});
  OperandType type2(Type::TENSOR_BOOL8, {2, 2});
  // Phase 1, operands
  auto input01 = model->addOperand(&type2);
  auto input11 = model->addOperand(&type16);
  auto input21 = model->addOperand(&type16);
  auto output01 = model->addOperand(&type16);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SELECT, {input01, input11, input21}, {output01});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input01, input11, input21},
    {output01});
  assert(model->isValid());
}

inline bool is_ignored_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_2(Model *model) {
  OperandType type2(Type::TENSOR_BOOL8, {2, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {2, 2});
  // Phase 1, operands
  auto input01 = model->addOperand(&type2);
  auto input11 = model->addOperand(&type3);
  auto input21 = model->addOperand(&type3);
  auto output01 = model->addOperand(&type3);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SELECT, {input01, input11, input21}, {output01});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input01, input11, input21},
    {output01});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant8_2(Model *model) {
  OperandType type17(Type::TENSOR_QUANT8_ASYMM, {2, 2}, 1.5f, 129);
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {2, 2}, 0.5f, 127);
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {2, 2}, 1.0f, 128);
  OperandType type2(Type::TENSOR_BOOL8, {2, 2});
  // Phase 1, operands
  auto input01 = model->addOperand(&type2);
  auto input11 = model->addOperand(&type17);
  auto input21 = model->addOperand(&type18);
  auto output01 = model->addOperand(&type19);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SELECT, {input01, input11, input21}, {output01});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input01, input11, input21},
    {output01});
  assert(model->isValid());
}

inline bool is_ignored_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_2(Model *model) {
  OperandType type2(Type::TENSOR_BOOL8, {2, 2});
  OperandType type20(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {2, 2});
  // Phase 1, operands
  auto input01 = model->addOperand(&type2);
  auto input11 = model->addOperand(&type3);
  auto input21 = model->addOperand(&type3);
  auto output01 = model->addOperand(&type20);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SELECT, {input01, input11, input21}, {output01});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input01, input11, input21},
    {output01});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_int32_2(Model *model) {
  OperandType type15(Type::TENSOR_INT32, {2, 2});
  OperandType type2(Type::TENSOR_BOOL8, {2, 2});
  OperandType type21(Type::TENSOR_INT32, {0, 0});
  // Phase 1, operands
  auto input01 = model->addOperand(&type2);
  auto input11 = model->addOperand(&type15);
  auto input21 = model->addOperand(&type15);
  auto output01 = model->addOperand(&type21);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SELECT, {input01, input11, input21}, {output01});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input01, input11, input21},
    {output01});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_int32_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_2(Model *model) {
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2});
  OperandType type2(Type::TENSOR_BOOL8, {2, 2});
  OperandType type22(Type::TENSOR_FLOAT16, {0, 0});
  // Phase 1, operands
  auto input01 = model->addOperand(&type2);
  auto input11 = model->addOperand(&type16);
  auto input21 = model->addOperand(&type16);
  auto output01 = model->addOperand(&type22);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SELECT, {input01, input11, input21}, {output01});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input01, input11, input21},
    {output01});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_2(Model *model) {
  OperandType type2(Type::TENSOR_BOOL8, {2, 2});
  OperandType type20(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {2, 2});
  // Phase 1, operands
  auto input01 = model->addOperand(&type2);
  auto input11 = model->addOperand(&type3);
  auto input21 = model->addOperand(&type3);
  auto output01 = model->addOperand(&type20);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SELECT, {input01, input11, input21}, {output01});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input01, input11, input21},
    {output01});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_quant8_2(Model *model) {
  OperandType type17(Type::TENSOR_QUANT8_ASYMM, {2, 2}, 1.5f, 129);
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {2, 2}, 0.5f, 127);
  OperandType type2(Type::TENSOR_BOOL8, {2, 2});
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {0, 0}, 1.0f, 128);
  // Phase 1, operands
  auto input01 = model->addOperand(&type2);
  auto input11 = model->addOperand(&type17);
  auto input21 = model->addOperand(&type18);
  auto output01 = model->addOperand(&type23);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SELECT, {input01, input11, input21}, {output01});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input01, input11, input21},
    {output01});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3(Model *model) {
  OperandType type4(Type::TENSOR_BOOL8, {2, 1, 2, 1, 2});
  OperandType type5(Type::TENSOR_FLOAT32, {2, 1, 2, 1, 2});
  // Phase 1, operands
  auto input02 = model->addOperand(&type4);
  auto input12 = model->addOperand(&type5);
  auto input22 = model->addOperand(&type5);
  auto output02 = model->addOperand(&type5);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SELECT, {input02, input12, input22}, {output02});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input02, input12, input22},
    {output02});
  assert(model->isValid());
}

inline bool is_ignored_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_int32_3(Model *model) {
  OperandType type24(Type::TENSOR_INT32, {2, 1, 2, 1, 2});
  OperandType type4(Type::TENSOR_BOOL8, {2, 1, 2, 1, 2});
  // Phase 1, operands
  auto input02 = model->addOperand(&type4);
  auto input12 = model->addOperand(&type24);
  auto input22 = model->addOperand(&type24);
  auto output02 = model->addOperand(&type24);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SELECT, {input02, input12, input22}, {output02});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input02, input12, input22},
    {output02});
  assert(model->isValid());
}

inline bool is_ignored_int32_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_3(Model *model) {
  OperandType type25(Type::TENSOR_FLOAT16, {2, 1, 2, 1, 2});
  OperandType type4(Type::TENSOR_BOOL8, {2, 1, 2, 1, 2});
  // Phase 1, operands
  auto input02 = model->addOperand(&type4);
  auto input12 = model->addOperand(&type25);
  auto input22 = model->addOperand(&type25);
  auto output02 = model->addOperand(&type25);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SELECT, {input02, input12, input22}, {output02});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input02, input12, input22},
    {output02});
  assert(model->isValid());
}

inline bool is_ignored_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_3(Model *model) {
  OperandType type4(Type::TENSOR_BOOL8, {2, 1, 2, 1, 2});
  OperandType type5(Type::TENSOR_FLOAT32, {2, 1, 2, 1, 2});
  // Phase 1, operands
  auto input02 = model->addOperand(&type4);
  auto input12 = model->addOperand(&type5);
  auto input22 = model->addOperand(&type5);
  auto output02 = model->addOperand(&type5);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SELECT, {input02, input12, input22}, {output02});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input02, input12, input22},
    {output02});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant8_3(Model *model) {
  OperandType type26(Type::TENSOR_QUANT8_ASYMM, {2, 1, 2, 1, 2}, 1.5f, 129);
  OperandType type27(Type::TENSOR_QUANT8_ASYMM, {2, 1, 2, 1, 2}, 0.5f, 127);
  OperandType type28(Type::TENSOR_QUANT8_ASYMM, {2, 1, 2, 1, 2}, 1.0f, 128);
  OperandType type4(Type::TENSOR_BOOL8, {2, 1, 2, 1, 2});
  // Phase 1, operands
  auto input02 = model->addOperand(&type4);
  auto input12 = model->addOperand(&type26);
  auto input22 = model->addOperand(&type27);
  auto output02 = model->addOperand(&type28);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SELECT, {input02, input12, input22}, {output02});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input02, input12, input22},
    {output02});
  assert(model->isValid());
}

inline bool is_ignored_quant8_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_3(Model *model) {
  OperandType type29(Type::TENSOR_FLOAT32, {0, 0, 0, 0, 0});
  OperandType type4(Type::TENSOR_BOOL8, {2, 1, 2, 1, 2});
  OperandType type5(Type::TENSOR_FLOAT32, {2, 1, 2, 1, 2});
  // Phase 1, operands
  auto input02 = model->addOperand(&type4);
  auto input12 = model->addOperand(&type5);
  auto input22 = model->addOperand(&type5);
  auto output02 = model->addOperand(&type29);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SELECT, {input02, input12, input22}, {output02});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input02, input12, input22},
    {output02});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_int32_3(Model *model) {
  OperandType type24(Type::TENSOR_INT32, {2, 1, 2, 1, 2});
  OperandType type30(Type::TENSOR_INT32, {0, 0, 0, 0, 0});
  OperandType type4(Type::TENSOR_BOOL8, {2, 1, 2, 1, 2});
  // Phase 1, operands
  auto input02 = model->addOperand(&type4);
  auto input12 = model->addOperand(&type24);
  auto input22 = model->addOperand(&type24);
  auto output02 = model->addOperand(&type30);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SELECT, {input02, input12, input22}, {output02});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input02, input12, input22},
    {output02});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_int32_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_3(Model *model) {
  OperandType type25(Type::TENSOR_FLOAT16, {2, 1, 2, 1, 2});
  OperandType type31(Type::TENSOR_FLOAT16, {0, 0, 0, 0, 0});
  OperandType type4(Type::TENSOR_BOOL8, {2, 1, 2, 1, 2});
  // Phase 1, operands
  auto input02 = model->addOperand(&type4);
  auto input12 = model->addOperand(&type25);
  auto input22 = model->addOperand(&type25);
  auto output02 = model->addOperand(&type31);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SELECT, {input02, input12, input22}, {output02});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input02, input12, input22},
    {output02});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_3(Model *model) {
  OperandType type29(Type::TENSOR_FLOAT32, {0, 0, 0, 0, 0});
  OperandType type4(Type::TENSOR_BOOL8, {2, 1, 2, 1, 2});
  OperandType type5(Type::TENSOR_FLOAT32, {2, 1, 2, 1, 2});
  // Phase 1, operands
  auto input02 = model->addOperand(&type4);
  auto input12 = model->addOperand(&type5);
  auto input22 = model->addOperand(&type5);
  auto output02 = model->addOperand(&type29);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SELECT, {input02, input12, input22}, {output02});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input02, input12, input22},
    {output02});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_quant8_3(Model *model) {
  OperandType type26(Type::TENSOR_QUANT8_ASYMM, {2, 1, 2, 1, 2}, 1.5f, 129);
  OperandType type27(Type::TENSOR_QUANT8_ASYMM, {2, 1, 2, 1, 2}, 0.5f, 127);
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0, 0}, 1.0f, 128);
  OperandType type4(Type::TENSOR_BOOL8, {2, 1, 2, 1, 2});
  // Phase 1, operands
  auto input02 = model->addOperand(&type4);
  auto input12 = model->addOperand(&type26);
  auto input22 = model->addOperand(&type27);
  auto output02 = model->addOperand(&type32);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_SELECT, {input02, input12, input22}, {output02});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input02, input12, input22},
    {output02});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_quant8_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

