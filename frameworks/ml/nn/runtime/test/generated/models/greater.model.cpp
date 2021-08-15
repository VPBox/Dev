// clang-format off
// Generated file (from: greater.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {3});
  OperandType type1(Type::TENSOR_BOOL8, {3});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto input1 = model->addOperand(&type0);
  auto output0 = model->addOperand(&type1);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_GREATER, {input0, input1}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_int32(Model *model) {
  OperandType type1(Type::TENSOR_BOOL8, {3});
  OperandType type12(Type::TENSOR_INT32, {3});
  // Phase 1, operands
  auto input0 = model->addOperand(&type12);
  auto input1 = model->addOperand(&type12);
  auto output0 = model->addOperand(&type1);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_GREATER, {input0, input1}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_int32(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16(Model *model) {
  OperandType type1(Type::TENSOR_BOOL8, {3});
  OperandType type13(Type::TENSOR_FLOAT16, {3});
  // Phase 1, operands
  auto input0 = model->addOperand(&type13);
  auto input1 = model->addOperand(&type13);
  auto output0 = model->addOperand(&type1);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_GREATER, {input0, input1}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {3});
  OperandType type1(Type::TENSOR_BOOL8, {3});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto input1 = model->addOperand(&type0);
  auto output0 = model->addOperand(&type1);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_GREATER, {input0, input1}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1},
    {output0});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {3});
  OperandType type14(Type::TENSOR_BOOL8, {0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto input1 = model->addOperand(&type0);
  auto output0 = model->addOperand(&type14);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_GREATER, {input0, input1}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_int32(Model *model) {
  OperandType type12(Type::TENSOR_INT32, {3});
  OperandType type14(Type::TENSOR_BOOL8, {0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type12);
  auto input1 = model->addOperand(&type12);
  auto output0 = model->addOperand(&type14);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_GREATER, {input0, input1}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_int32(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16(Model *model) {
  OperandType type13(Type::TENSOR_FLOAT16, {3});
  OperandType type14(Type::TENSOR_BOOL8, {0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type13);
  auto input1 = model->addOperand(&type13);
  auto output0 = model->addOperand(&type14);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_GREATER, {input0, input1}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {3});
  OperandType type14(Type::TENSOR_BOOL8, {0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto input1 = model->addOperand(&type0);
  auto output0 = model->addOperand(&type14);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_GREATER, {input0, input1}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, input1},
    {output0});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_2(Model *model) {
  OperandType type2(Type::TENSOR_FLOAT32, {2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::TENSOR_BOOL8, {2, 2});
  // Phase 1, operands
  auto input01 = model->addOperand(&type2);
  auto input11 = model->addOperand(&type3);
  auto output01 = model->addOperand(&type4);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_GREATER, {input01, input11}, {output01});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input01, input11},
    {output01});
  assert(model->isValid());
}

inline bool is_ignored_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_int32_2(Model *model) {
  OperandType type15(Type::TENSOR_INT32, {2, 1});
  OperandType type16(Type::TENSOR_INT32, {2});
  OperandType type4(Type::TENSOR_BOOL8, {2, 2});
  // Phase 1, operands
  auto input01 = model->addOperand(&type15);
  auto input11 = model->addOperand(&type16);
  auto output01 = model->addOperand(&type4);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_GREATER, {input01, input11}, {output01});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input01, input11},
    {output01});
  assert(model->isValid());
}

inline bool is_ignored_int32_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_2(Model *model) {
  OperandType type17(Type::TENSOR_FLOAT16, {2, 1});
  OperandType type18(Type::TENSOR_FLOAT16, {2});
  OperandType type4(Type::TENSOR_BOOL8, {2, 2});
  // Phase 1, operands
  auto input01 = model->addOperand(&type17);
  auto input11 = model->addOperand(&type18);
  auto output01 = model->addOperand(&type4);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_GREATER, {input01, input11}, {output01});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input01, input11},
    {output01});
  assert(model->isValid());
}

inline bool is_ignored_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_2(Model *model) {
  OperandType type2(Type::TENSOR_FLOAT32, {2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::TENSOR_BOOL8, {2, 2});
  // Phase 1, operands
  auto input01 = model->addOperand(&type2);
  auto input11 = model->addOperand(&type3);
  auto output01 = model->addOperand(&type4);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_GREATER, {input01, input11}, {output01});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input01, input11},
    {output01});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_2(Model *model) {
  OperandType type19(Type::TENSOR_BOOL8, {0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  // Phase 1, operands
  auto input01 = model->addOperand(&type2);
  auto input11 = model->addOperand(&type3);
  auto output01 = model->addOperand(&type19);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_GREATER, {input01, input11}, {output01});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input01, input11},
    {output01});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_int32_2(Model *model) {
  OperandType type15(Type::TENSOR_INT32, {2, 1});
  OperandType type16(Type::TENSOR_INT32, {2});
  OperandType type19(Type::TENSOR_BOOL8, {0, 0});
  // Phase 1, operands
  auto input01 = model->addOperand(&type15);
  auto input11 = model->addOperand(&type16);
  auto output01 = model->addOperand(&type19);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_GREATER, {input01, input11}, {output01});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input01, input11},
    {output01});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_int32_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_2(Model *model) {
  OperandType type17(Type::TENSOR_FLOAT16, {2, 1});
  OperandType type18(Type::TENSOR_FLOAT16, {2});
  OperandType type19(Type::TENSOR_BOOL8, {0, 0});
  // Phase 1, operands
  auto input01 = model->addOperand(&type17);
  auto input11 = model->addOperand(&type18);
  auto output01 = model->addOperand(&type19);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_GREATER, {input01, input11}, {output01});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input01, input11},
    {output01});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_2(Model *model) {
  OperandType type19(Type::TENSOR_BOOL8, {0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  // Phase 1, operands
  auto input01 = model->addOperand(&type2);
  auto input11 = model->addOperand(&type3);
  auto output01 = model->addOperand(&type19);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_GREATER, {input01, input11}, {output01});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input01, input11},
    {output01});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3(Model *model) {
  OperandType type1(Type::TENSOR_BOOL8, {3});
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {3}, 1.0f, 128);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {1}, 2.0f, 128);
  // Phase 1, operands
  auto input02 = model->addOperand(&type5);
  auto input12 = model->addOperand(&type6);
  auto output02 = model->addOperand(&type1);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_GREATER, {input02, input12}, {output02});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input02, input12},
    {output02});
  assert(model->isValid());
}

inline bool is_ignored_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_3(Model *model) {
  OperandType type14(Type::TENSOR_BOOL8, {0});
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {3}, 1.0f, 128);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {1}, 2.0f, 128);
  // Phase 1, operands
  auto input02 = model->addOperand(&type5);
  auto input12 = model->addOperand(&type6);
  auto output02 = model->addOperand(&type14);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_GREATER, {input02, input12}, {output02});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input02, input12},
    {output02});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_4(Model *model) {
  OperandType type1(Type::TENSOR_BOOL8, {3});
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {3}, 1.0f, 128);
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {1}, 1.0f, 129);
  // Phase 1, operands
  auto input03 = model->addOperand(&type5);
  auto input13 = model->addOperand(&type7);
  auto output03 = model->addOperand(&type1);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_GREATER, {input03, input13}, {output03});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input03, input13},
    {output03});
  assert(model->isValid());
}

inline bool is_ignored_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_4(Model *model) {
  OperandType type14(Type::TENSOR_BOOL8, {0});
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {3}, 1.0f, 128);
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {1}, 1.0f, 129);
  // Phase 1, operands
  auto input03 = model->addOperand(&type5);
  auto input13 = model->addOperand(&type7);
  auto output03 = model->addOperand(&type14);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_GREATER, {input03, input13}, {output03});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input03, input13},
    {output03});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_5(Model *model) {
  OperandType type10(Type::TENSOR_BOOL8, {1});
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1}, 1.64771f, 31);
  OperandType type9(Type::TENSOR_QUANT8_ASYMM, {1}, 1.49725f, 240);
  // Phase 1, operands
  auto input04 = model->addOperand(&type8);
  auto input14 = model->addOperand(&type9);
  auto output04 = model->addOperand(&type10);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_GREATER, {input04, input14}, {output04});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input04, input14},
    {output04});
  assert(model->isValid());
}

inline bool is_ignored_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_5(Model *model) {
  OperandType type14(Type::TENSOR_BOOL8, {0});
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1}, 1.64771f, 31);
  OperandType type9(Type::TENSOR_QUANT8_ASYMM, {1}, 1.49725f, 240);
  // Phase 1, operands
  auto input04 = model->addOperand(&type8);
  auto input14 = model->addOperand(&type9);
  auto output04 = model->addOperand(&type14);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_GREATER, {input04, input14}, {output04});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input04, input14},
    {output04});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_6(Model *model) {
  OperandType type10(Type::TENSOR_BOOL8, {1});
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1}, 1.64771f, 31);
  OperandType type9(Type::TENSOR_QUANT8_ASYMM, {1}, 1.49725f, 240);
  // Phase 1, operands
  auto input05 = model->addOperand(&type9);
  auto input15 = model->addOperand(&type8);
  auto output05 = model->addOperand(&type10);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_GREATER, {input05, input15}, {output05});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input05, input15},
    {output05});
  assert(model->isValid());
}

inline bool is_ignored_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_6(Model *model) {
  OperandType type14(Type::TENSOR_BOOL8, {0});
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1}, 1.64771f, 31);
  OperandType type9(Type::TENSOR_QUANT8_ASYMM, {1}, 1.49725f, 240);
  // Phase 1, operands
  auto input05 = model->addOperand(&type9);
  auto input15 = model->addOperand(&type8);
  auto output05 = model->addOperand(&type14);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_GREATER, {input05, input15}, {output05});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input05, input15},
    {output05});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_7(Model *model) {
  OperandType type11(Type::TENSOR_BOOL8, {4});
  // Phase 1, operands
  auto input06 = model->addOperand(&type11);
  auto input16 = model->addOperand(&type11);
  auto output06 = model->addOperand(&type11);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_GREATER, {input06, input16}, {output06});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input06, input16},
    {output06});
  assert(model->isValid());
}

inline bool is_ignored_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_7(Model *model) {
  OperandType type11(Type::TENSOR_BOOL8, {4});
  OperandType type14(Type::TENSOR_BOOL8, {0});
  // Phase 1, operands
  auto input06 = model->addOperand(&type11);
  auto input16 = model->addOperand(&type11);
  auto output06 = model->addOperand(&type14);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_GREATER, {input06, input16}, {output06});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input06, input16},
    {output06});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_7(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

