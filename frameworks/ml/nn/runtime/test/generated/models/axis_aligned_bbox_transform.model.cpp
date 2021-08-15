// clang-format off
// Generated file (from: axis_aligned_bbox_transform.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {5, 4});
  OperandType type1(Type::TENSOR_FLOAT32, {5, 8});
  OperandType type2(Type::TENSOR_INT32, {5});
  OperandType type3(Type::TENSOR_FLOAT32, {4, 2});
  // Phase 1, operands
  auto roi = model->addOperand(&type0);
  auto bboxDeltas = model->addOperand(&type1);
  auto batchSplit = model->addOperand(&type2);
  auto imageInfo = model->addOperand(&type3);
  auto out = model->addOperand(&type1);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_AXIS_ALIGNED_BBOX_TRANSFORM, {roi, bboxDeltas, batchSplit, imageInfo}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {roi, bboxDeltas, batchSplit, imageInfo},
    {out});
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {5, 4});
  OperandType type1(Type::TENSOR_FLOAT32, {5, 8});
  OperandType type2(Type::TENSOR_INT32, {5});
  OperandType type3(Type::TENSOR_FLOAT32, {4, 2});
  // Phase 1, operands
  auto roi = model->addOperand(&type0);
  auto bboxDeltas = model->addOperand(&type1);
  auto batchSplit = model->addOperand(&type2);
  auto imageInfo = model->addOperand(&type3);
  auto out = model->addOperand(&type1);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_AXIS_ALIGNED_BBOX_TRANSFORM, {roi, bboxDeltas, batchSplit, imageInfo}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {roi, bboxDeltas, batchSplit, imageInfo},
    {out});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16(Model *model) {
  OperandType type2(Type::TENSOR_INT32, {5});
  OperandType type5(Type::TENSOR_FLOAT16, {5, 8});
  OperandType type6(Type::TENSOR_FLOAT16, {4, 2});
  OperandType type7(Type::TENSOR_FLOAT16, {5, 4});
  // Phase 1, operands
  auto roi = model->addOperand(&type7);
  auto bboxDeltas = model->addOperand(&type5);
  auto batchSplit = model->addOperand(&type2);
  auto imageInfo = model->addOperand(&type6);
  auto out = model->addOperand(&type5);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_AXIS_ALIGNED_BBOX_TRANSFORM, {roi, bboxDeltas, batchSplit, imageInfo}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {roi, bboxDeltas, batchSplit, imageInfo},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant8(Model *model) {
  OperandType type10(Type::TENSOR_QUANT16_ASYMM, {5, 8}, 0.125f, 0);
  OperandType type11(Type::TENSOR_QUANT16_ASYMM, {5, 4}, 0.125f, 0);
  OperandType type2(Type::TENSOR_INT32, {5});
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {5, 8}, 0.05f, 128);
  OperandType type9(Type::TENSOR_QUANT16_ASYMM, {4, 2}, 0.125f, 0);
  // Phase 1, operands
  auto roi = model->addOperand(&type11);
  auto bboxDeltas = model->addOperand(&type8);
  auto batchSplit = model->addOperand(&type2);
  auto imageInfo = model->addOperand(&type9);
  auto out = model->addOperand(&type10);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_AXIS_ALIGNED_BBOX_TRANSFORM, {roi, bboxDeltas, batchSplit, imageInfo}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {roi, bboxDeltas, batchSplit, imageInfo},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {5, 4});
  OperandType type1(Type::TENSOR_FLOAT32, {5, 8});
  OperandType type12(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type2(Type::TENSOR_INT32, {5});
  OperandType type3(Type::TENSOR_FLOAT32, {4, 2});
  // Phase 1, operands
  auto roi = model->addOperand(&type0);
  auto bboxDeltas = model->addOperand(&type1);
  auto batchSplit = model->addOperand(&type2);
  auto imageInfo = model->addOperand(&type3);
  auto out = model->addOperand(&type12);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_AXIS_ALIGNED_BBOX_TRANSFORM, {roi, bboxDeltas, batchSplit, imageInfo}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {roi, bboxDeltas, batchSplit, imageInfo},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {5, 4});
  OperandType type1(Type::TENSOR_FLOAT32, {5, 8});
  OperandType type12(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type2(Type::TENSOR_INT32, {5});
  OperandType type3(Type::TENSOR_FLOAT32, {4, 2});
  // Phase 1, operands
  auto roi = model->addOperand(&type0);
  auto bboxDeltas = model->addOperand(&type1);
  auto batchSplit = model->addOperand(&type2);
  auto imageInfo = model->addOperand(&type3);
  auto out = model->addOperand(&type12);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_AXIS_ALIGNED_BBOX_TRANSFORM, {roi, bboxDeltas, batchSplit, imageInfo}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {roi, bboxDeltas, batchSplit, imageInfo},
    {out});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16(Model *model) {
  OperandType type13(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type2(Type::TENSOR_INT32, {5});
  OperandType type5(Type::TENSOR_FLOAT16, {5, 8});
  OperandType type6(Type::TENSOR_FLOAT16, {4, 2});
  OperandType type7(Type::TENSOR_FLOAT16, {5, 4});
  // Phase 1, operands
  auto roi = model->addOperand(&type7);
  auto bboxDeltas = model->addOperand(&type5);
  auto batchSplit = model->addOperand(&type2);
  auto imageInfo = model->addOperand(&type6);
  auto out = model->addOperand(&type13);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_AXIS_ALIGNED_BBOX_TRANSFORM, {roi, bboxDeltas, batchSplit, imageInfo}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {roi, bboxDeltas, batchSplit, imageInfo},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_quant8(Model *model) {
  OperandType type11(Type::TENSOR_QUANT16_ASYMM, {5, 4}, 0.125f, 0);
  OperandType type14(Type::TENSOR_QUANT16_ASYMM, {0, 0}, 0.125f, 0);
  OperandType type2(Type::TENSOR_INT32, {5});
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {5, 8}, 0.05f, 128);
  OperandType type9(Type::TENSOR_QUANT16_ASYMM, {4, 2}, 0.125f, 0);
  // Phase 1, operands
  auto roi = model->addOperand(&type11);
  auto bboxDeltas = model->addOperand(&type8);
  auto batchSplit = model->addOperand(&type2);
  auto imageInfo = model->addOperand(&type9);
  auto out = model->addOperand(&type14);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_AXIS_ALIGNED_BBOX_TRANSFORM, {roi, bboxDeltas, batchSplit, imageInfo}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {roi, bboxDeltas, batchSplit, imageInfo},
    {out});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {5, 4});
  OperandType type1(Type::TENSOR_FLOAT32, {5, 8});
  OperandType type2(Type::TENSOR_INT32, {5});
  OperandType type4(Type::TENSOR_FLOAT32, {7, 2});
  // Phase 1, operands
  auto roi1 = model->addOperand(&type0);
  auto bboxDeltas1 = model->addOperand(&type1);
  auto batchSplit1 = model->addOperand(&type2);
  auto imageInfo1 = model->addOperand(&type4);
  auto out1 = model->addOperand(&type1);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_AXIS_ALIGNED_BBOX_TRANSFORM, {roi1, bboxDeltas1, batchSplit1, imageInfo1}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {roi1, bboxDeltas1, batchSplit1, imageInfo1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {5, 4});
  OperandType type1(Type::TENSOR_FLOAT32, {5, 8});
  OperandType type2(Type::TENSOR_INT32, {5});
  OperandType type4(Type::TENSOR_FLOAT32, {7, 2});
  // Phase 1, operands
  auto roi1 = model->addOperand(&type0);
  auto bboxDeltas1 = model->addOperand(&type1);
  auto batchSplit1 = model->addOperand(&type2);
  auto imageInfo1 = model->addOperand(&type4);
  auto out1 = model->addOperand(&type1);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_AXIS_ALIGNED_BBOX_TRANSFORM, {roi1, bboxDeltas1, batchSplit1, imageInfo1}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {roi1, bboxDeltas1, batchSplit1, imageInfo1},
    {out1});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_2(Model *model) {
  OperandType type15(Type::TENSOR_FLOAT16, {7, 2});
  OperandType type2(Type::TENSOR_INT32, {5});
  OperandType type5(Type::TENSOR_FLOAT16, {5, 8});
  OperandType type7(Type::TENSOR_FLOAT16, {5, 4});
  // Phase 1, operands
  auto roi1 = model->addOperand(&type7);
  auto bboxDeltas1 = model->addOperand(&type5);
  auto batchSplit1 = model->addOperand(&type2);
  auto imageInfo1 = model->addOperand(&type15);
  auto out1 = model->addOperand(&type5);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_AXIS_ALIGNED_BBOX_TRANSFORM, {roi1, bboxDeltas1, batchSplit1, imageInfo1}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {roi1, bboxDeltas1, batchSplit1, imageInfo1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant8_2(Model *model) {
  OperandType type10(Type::TENSOR_QUANT16_ASYMM, {5, 8}, 0.125f, 0);
  OperandType type11(Type::TENSOR_QUANT16_ASYMM, {5, 4}, 0.125f, 0);
  OperandType type16(Type::TENSOR_QUANT16_ASYMM, {7, 2}, 0.125f, 0);
  OperandType type2(Type::TENSOR_INT32, {5});
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {5, 8}, 0.05f, 128);
  // Phase 1, operands
  auto roi1 = model->addOperand(&type11);
  auto bboxDeltas1 = model->addOperand(&type8);
  auto batchSplit1 = model->addOperand(&type2);
  auto imageInfo1 = model->addOperand(&type16);
  auto out1 = model->addOperand(&type10);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_AXIS_ALIGNED_BBOX_TRANSFORM, {roi1, bboxDeltas1, batchSplit1, imageInfo1}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {roi1, bboxDeltas1, batchSplit1, imageInfo1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {5, 4});
  OperandType type1(Type::TENSOR_FLOAT32, {5, 8});
  OperandType type12(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type2(Type::TENSOR_INT32, {5});
  OperandType type4(Type::TENSOR_FLOAT32, {7, 2});
  // Phase 1, operands
  auto roi1 = model->addOperand(&type0);
  auto bboxDeltas1 = model->addOperand(&type1);
  auto batchSplit1 = model->addOperand(&type2);
  auto imageInfo1 = model->addOperand(&type4);
  auto out1 = model->addOperand(&type12);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_AXIS_ALIGNED_BBOX_TRANSFORM, {roi1, bboxDeltas1, batchSplit1, imageInfo1}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {roi1, bboxDeltas1, batchSplit1, imageInfo1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {5, 4});
  OperandType type1(Type::TENSOR_FLOAT32, {5, 8});
  OperandType type12(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type2(Type::TENSOR_INT32, {5});
  OperandType type4(Type::TENSOR_FLOAT32, {7, 2});
  // Phase 1, operands
  auto roi1 = model->addOperand(&type0);
  auto bboxDeltas1 = model->addOperand(&type1);
  auto batchSplit1 = model->addOperand(&type2);
  auto imageInfo1 = model->addOperand(&type4);
  auto out1 = model->addOperand(&type12);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_AXIS_ALIGNED_BBOX_TRANSFORM, {roi1, bboxDeltas1, batchSplit1, imageInfo1}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {roi1, bboxDeltas1, batchSplit1, imageInfo1},
    {out1});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_2(Model *model) {
  OperandType type13(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type15(Type::TENSOR_FLOAT16, {7, 2});
  OperandType type2(Type::TENSOR_INT32, {5});
  OperandType type5(Type::TENSOR_FLOAT16, {5, 8});
  OperandType type7(Type::TENSOR_FLOAT16, {5, 4});
  // Phase 1, operands
  auto roi1 = model->addOperand(&type7);
  auto bboxDeltas1 = model->addOperand(&type5);
  auto batchSplit1 = model->addOperand(&type2);
  auto imageInfo1 = model->addOperand(&type15);
  auto out1 = model->addOperand(&type13);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_AXIS_ALIGNED_BBOX_TRANSFORM, {roi1, bboxDeltas1, batchSplit1, imageInfo1}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {roi1, bboxDeltas1, batchSplit1, imageInfo1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_quant8_2(Model *model) {
  OperandType type11(Type::TENSOR_QUANT16_ASYMM, {5, 4}, 0.125f, 0);
  OperandType type14(Type::TENSOR_QUANT16_ASYMM, {0, 0}, 0.125f, 0);
  OperandType type16(Type::TENSOR_QUANT16_ASYMM, {7, 2}, 0.125f, 0);
  OperandType type2(Type::TENSOR_INT32, {5});
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {5, 8}, 0.05f, 128);
  // Phase 1, operands
  auto roi1 = model->addOperand(&type11);
  auto bboxDeltas1 = model->addOperand(&type8);
  auto batchSplit1 = model->addOperand(&type2);
  auto imageInfo1 = model->addOperand(&type16);
  auto out1 = model->addOperand(&type14);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_AXIS_ALIGNED_BBOX_TRANSFORM, {roi1, bboxDeltas1, batchSplit1, imageInfo1}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {roi1, bboxDeltas1, batchSplit1, imageInfo1},
    {out1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

