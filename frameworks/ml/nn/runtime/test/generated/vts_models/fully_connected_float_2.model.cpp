// clang-format off
// Generated file (from: fully_connected_float_2.mod.py). Do not edit
// Create the model
Model createTestModel() {
    const std::vector<Operand> operands = {
        {
            .type = OperandType::TENSOR_FLOAT32,
            .dimensions = {2, 8},
            .numberOfConsumers = 1,
            .scale = 0.0f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_FLOAT32,
            .dimensions = {16, 8},
            .numberOfConsumers = 1,
            .scale = 0.0f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 0, .length = 512},
        },
        {
            .type = OperandType::TENSOR_FLOAT32,
            .dimensions = {16},
            .numberOfConsumers = 1,
            .scale = 0.0f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 512, .length = 64},
        },
        {
            .type = OperandType::INT32,
            .dimensions = {},
            .numberOfConsumers = 1,
            .scale = 0.0f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 576, .length = 4},
        },
        {
            .type = OperandType::TENSOR_FLOAT32,
            .dimensions = {2, 16},
            .numberOfConsumers = 0,
            .scale = 0.0f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_OUTPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        }
    };

    const std::vector<Operation> operations = {
        {
            .type = OperationType::FULLY_CONNECTED,
            .inputs = {0, 1, 2, 3},
            .outputs = {4},
        }
    };

    const std::vector<uint32_t> inputIndexes = {0};
    const std::vector<uint32_t> outputIndexes = {4};
    std::vector<uint8_t> operandValues = {
      166, 9, 187, 61, 142, 177, 211, 61, 242, 12, 162, 190, 210, 58, 170, 189, 103, 243, 24, 62, 243, 90, 73, 190, 191, 71, 253, 189, 73, 157, 128, 61, 102, 22, 129, 61, 36, 69, 68, 62, 195, 244, 125, 189, 158, 235, 123, 189, 244, 24, 141, 190, 213, 64, 115, 61, 142, 174, 242, 189, 49, 64, 162, 189, 205, 200, 224, 61, 21, 26, 8, 60, 80, 82, 128, 189, 74, 179, 121, 189, 54, 232, 75, 189, 126, 143, 218, 189, 37, 147, 163, 190, 116, 70, 212, 189, 202, 108, 136, 62, 125, 3, 83, 61, 78, 210, 252, 189, 0, 26, 165, 62, 119, 72, 49, 61, 233, 243, 49, 190, 61, 243, 66, 190, 137, 236, 3, 59, 189, 200, 196, 61, 5, 78, 6, 62, 165, 75, 255, 60, 76, 27, 206, 61, 128, 215, 159, 190, 245, 186, 165, 189, 208, 127, 207, 189, 82, 211, 238, 189, 215, 166, 17, 62, 226, 59, 177, 61, 99, 126, 14, 190, 80, 115, 50, 62, 111, 213, 149, 189, 202, 252, 163, 61, 75, 63, 225, 59, 29, 227, 106, 62, 64, 166, 213, 189, 133, 66, 52, 190, 213, 151, 85, 190, 247, 143, 133, 186, 183, 235, 37, 190, 14, 135, 165, 61, 215, 248, 204, 188, 124, 126, 152, 61, 34, 84, 129, 190, 203, 161, 37, 190, 58, 173, 11, 190, 71, 30, 8, 60, 244, 26, 187, 59, 57, 214, 197, 61, 199, 101, 148, 62, 77, 17, 96, 62, 55, 251, 155, 190, 110, 82, 145, 189, 39, 162, 95, 61, 202, 164, 22, 62, 96, 115, 14, 190, 30, 23, 149, 189, 74, 65, 39, 190, 71, 228, 43, 190, 48, 218, 3, 190, 13, 142, 18, 189, 15, 39, 240, 61, 200, 182, 140, 60, 216, 211, 134, 62, 199, 217, 52, 190, 211, 162, 134, 62, 171, 205, 191, 189, 128, 182, 173, 190, 72, 136, 170, 62, 183, 42, 57, 62, 20, 62, 147, 62, 130, 226, 135, 61, 158, 178, 74, 190, 61, 100, 234, 189, 41, 179, 113, 190, 150, 8, 212, 61, 97, 109, 12, 189, 140, 19, 191, 61, 132, 217, 20, 190, 39, 48, 93, 61, 182, 74, 128, 62, 59, 199, 32, 62, 108, 236, 18, 190, 2, 102, 14, 190, 14, 47, 72, 189, 83, 91, 138, 61, 250, 95, 14, 62, 57, 241, 165, 62, 112, 125, 104, 62, 213, 204, 26, 61, 57, 42, 103, 190, 249, 106, 119, 190, 38, 54, 183, 62, 50, 5, 11, 62, 44, 16, 157, 190, 109, 1, 1, 190, 117, 62, 188, 61, 23, 126, 136, 62, 173, 221, 246, 60, 123, 193, 135, 62, 171, 6, 17, 62, 179, 210, 164, 60, 51, 167, 171, 61, 55, 23, 255, 189, 208, 13, 205, 189, 95, 66, 69, 189, 39, 133, 57, 62, 63, 200, 154, 62, 178, 71, 40, 189, 149, 216, 21, 62, 245, 242, 123, 189, 1, 48, 110, 190, 179, 9, 144, 62, 214, 229, 20, 62, 248, 56, 99, 190, 193, 114, 36, 190, 95, 181, 82, 62, 108, 95, 160, 189, 19, 182, 159, 189, 239, 226, 253, 58, 213, 236, 129, 60, 21, 198, 22, 61, 225, 70, 74, 60, 245, 189, 134, 58, 167, 231, 29, 61, 46, 115, 154, 61, 38, 109, 170, 60, 222, 144, 70, 61, 140, 45, 4, 189, 142, 231, 51, 62, 143, 111, 175, 189, 1, 0, 0, 0
    };
    const std::vector<hidl_memory> pools = {};

    return {
        .operands = operands,
        .operations = operations,
        .inputIndexes = inputIndexes,
        .outputIndexes = outputIndexes,
        .operandValues = operandValues,
        .pools = pools,
    };
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

// Create the model
Model createTestModel_dynamic_output_shape() {
    const std::vector<Operand> operands = {
        {
            .type = OperandType::TENSOR_FLOAT32,
            .dimensions = {2, 8},
            .numberOfConsumers = 1,
            .scale = 0.0f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_FLOAT32,
            .dimensions = {16, 8},
            .numberOfConsumers = 1,
            .scale = 0.0f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 0, .length = 512},
        },
        {
            .type = OperandType::TENSOR_FLOAT32,
            .dimensions = {16},
            .numberOfConsumers = 1,
            .scale = 0.0f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 512, .length = 64},
        },
        {
            .type = OperandType::INT32,
            .dimensions = {},
            .numberOfConsumers = 1,
            .scale = 0.0f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 576, .length = 4},
        },
        {
            .type = OperandType::TENSOR_FLOAT32,
            .dimensions = {0, 0},
            .numberOfConsumers = 0,
            .scale = 0.0f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_OUTPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        }
    };

    const std::vector<Operation> operations = {
        {
            .type = OperationType::FULLY_CONNECTED,
            .inputs = {0, 1, 2, 3},
            .outputs = {4},
        }
    };

    const std::vector<uint32_t> inputIndexes = {0};
    const std::vector<uint32_t> outputIndexes = {4};
    std::vector<uint8_t> operandValues = {
      166, 9, 187, 61, 142, 177, 211, 61, 242, 12, 162, 190, 210, 58, 170, 189, 103, 243, 24, 62, 243, 90, 73, 190, 191, 71, 253, 189, 73, 157, 128, 61, 102, 22, 129, 61, 36, 69, 68, 62, 195, 244, 125, 189, 158, 235, 123, 189, 244, 24, 141, 190, 213, 64, 115, 61, 142, 174, 242, 189, 49, 64, 162, 189, 205, 200, 224, 61, 21, 26, 8, 60, 80, 82, 128, 189, 74, 179, 121, 189, 54, 232, 75, 189, 126, 143, 218, 189, 37, 147, 163, 190, 116, 70, 212, 189, 202, 108, 136, 62, 125, 3, 83, 61, 78, 210, 252, 189, 0, 26, 165, 62, 119, 72, 49, 61, 233, 243, 49, 190, 61, 243, 66, 190, 137, 236, 3, 59, 189, 200, 196, 61, 5, 78, 6, 62, 165, 75, 255, 60, 76, 27, 206, 61, 128, 215, 159, 190, 245, 186, 165, 189, 208, 127, 207, 189, 82, 211, 238, 189, 215, 166, 17, 62, 226, 59, 177, 61, 99, 126, 14, 190, 80, 115, 50, 62, 111, 213, 149, 189, 202, 252, 163, 61, 75, 63, 225, 59, 29, 227, 106, 62, 64, 166, 213, 189, 133, 66, 52, 190, 213, 151, 85, 190, 247, 143, 133, 186, 183, 235, 37, 190, 14, 135, 165, 61, 215, 248, 204, 188, 124, 126, 152, 61, 34, 84, 129, 190, 203, 161, 37, 190, 58, 173, 11, 190, 71, 30, 8, 60, 244, 26, 187, 59, 57, 214, 197, 61, 199, 101, 148, 62, 77, 17, 96, 62, 55, 251, 155, 190, 110, 82, 145, 189, 39, 162, 95, 61, 202, 164, 22, 62, 96, 115, 14, 190, 30, 23, 149, 189, 74, 65, 39, 190, 71, 228, 43, 190, 48, 218, 3, 190, 13, 142, 18, 189, 15, 39, 240, 61, 200, 182, 140, 60, 216, 211, 134, 62, 199, 217, 52, 190, 211, 162, 134, 62, 171, 205, 191, 189, 128, 182, 173, 190, 72, 136, 170, 62, 183, 42, 57, 62, 20, 62, 147, 62, 130, 226, 135, 61, 158, 178, 74, 190, 61, 100, 234, 189, 41, 179, 113, 190, 150, 8, 212, 61, 97, 109, 12, 189, 140, 19, 191, 61, 132, 217, 20, 190, 39, 48, 93, 61, 182, 74, 128, 62, 59, 199, 32, 62, 108, 236, 18, 190, 2, 102, 14, 190, 14, 47, 72, 189, 83, 91, 138, 61, 250, 95, 14, 62, 57, 241, 165, 62, 112, 125, 104, 62, 213, 204, 26, 61, 57, 42, 103, 190, 249, 106, 119, 190, 38, 54, 183, 62, 50, 5, 11, 62, 44, 16, 157, 190, 109, 1, 1, 190, 117, 62, 188, 61, 23, 126, 136, 62, 173, 221, 246, 60, 123, 193, 135, 62, 171, 6, 17, 62, 179, 210, 164, 60, 51, 167, 171, 61, 55, 23, 255, 189, 208, 13, 205, 189, 95, 66, 69, 189, 39, 133, 57, 62, 63, 200, 154, 62, 178, 71, 40, 189, 149, 216, 21, 62, 245, 242, 123, 189, 1, 48, 110, 190, 179, 9, 144, 62, 214, 229, 20, 62, 248, 56, 99, 190, 193, 114, 36, 190, 95, 181, 82, 62, 108, 95, 160, 189, 19, 182, 159, 189, 239, 226, 253, 58, 213, 236, 129, 60, 21, 198, 22, 61, 225, 70, 74, 60, 245, 189, 134, 58, 167, 231, 29, 61, 46, 115, 154, 61, 38, 109, 170, 60, 222, 144, 70, 61, 140, 45, 4, 189, 142, 231, 51, 62, 143, 111, 175, 189, 1, 0, 0, 0
    };
    const std::vector<hidl_memory> pools = {};

    return {
        .operands = operands,
        .operations = operations,
        .inputIndexes = inputIndexes,
        .outputIndexes = outputIndexes,
        .operandValues = operandValues,
        .pools = pools,
    };
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

