// clang-format off
// Generated file (from: quantized_lstm.mod.py). Do not edit
// Create the model
Model createTestModel() {
    const std::vector<Operand> operands = {
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {2, 2},
            .numberOfConsumers = 1,
            .scale = 0.0078125f,
            .zeroPoint = 128,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 2},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 2},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 2},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 2},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 4},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 4},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 4},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 4},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_INT32,
            .dimensions = {4},
            .numberOfConsumers = 1,
            .scale = 3.1876640625e-05f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_INT32,
            .dimensions = {4},
            .numberOfConsumers = 1,
            .scale = 3.1876640625e-05f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_INT32,
            .dimensions = {4},
            .numberOfConsumers = 1,
            .scale = 3.1876640625e-05f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_INT32,
            .dimensions = {4},
            .numberOfConsumers = 1,
            .scale = 3.1876640625e-05f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT16_SYMM,
            .dimensions = {2, 4},
            .numberOfConsumers = 1,
            .scale = 0.00048828125f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {2, 4},
            .numberOfConsumers = 1,
            .scale = 0.0078125f,
            .zeroPoint = 128,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT16_SYMM,
            .dimensions = {2, 4},
            .numberOfConsumers = 0,
            .scale = 0.00048828125f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_OUTPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {2, 4},
            .numberOfConsumers = 0,
            .scale = 0.0078125f,
            .zeroPoint = 128,
            .lifetime = OperandLifeTime::MODEL_OUTPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        }
    };

    const std::vector<Operation> operations = {
        {
            .type = OperationType::QUANTIZED_16BIT_LSTM,
            .inputs = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14},
            .outputs = {15, 16},
        }
    };

    const std::vector<uint32_t> inputIndexes = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    const std::vector<uint32_t> outputIndexes = {15, 16};
    std::vector<uint8_t> operandValues = {};
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
Model createTestModel_relaxed() {
    const std::vector<Operand> operands = {
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {2, 2},
            .numberOfConsumers = 1,
            .scale = 0.0078125f,
            .zeroPoint = 128,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 2},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 2},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 2},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 2},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 4},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 4},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 4},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 4},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_INT32,
            .dimensions = {4},
            .numberOfConsumers = 1,
            .scale = 3.1876640625e-05f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_INT32,
            .dimensions = {4},
            .numberOfConsumers = 1,
            .scale = 3.1876640625e-05f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_INT32,
            .dimensions = {4},
            .numberOfConsumers = 1,
            .scale = 3.1876640625e-05f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_INT32,
            .dimensions = {4},
            .numberOfConsumers = 1,
            .scale = 3.1876640625e-05f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT16_SYMM,
            .dimensions = {2, 4},
            .numberOfConsumers = 1,
            .scale = 0.00048828125f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {2, 4},
            .numberOfConsumers = 1,
            .scale = 0.0078125f,
            .zeroPoint = 128,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT16_SYMM,
            .dimensions = {2, 4},
            .numberOfConsumers = 0,
            .scale = 0.00048828125f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_OUTPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {2, 4},
            .numberOfConsumers = 0,
            .scale = 0.0078125f,
            .zeroPoint = 128,
            .lifetime = OperandLifeTime::MODEL_OUTPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        }
    };

    const std::vector<Operation> operations = {
        {
            .type = OperationType::QUANTIZED_16BIT_LSTM,
            .inputs = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14},
            .outputs = {15, 16},
        }
    };

    const std::vector<uint32_t> inputIndexes = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    const std::vector<uint32_t> outputIndexes = {15, 16};
    std::vector<uint8_t> operandValues = {};
    const std::vector<hidl_memory> pools = {};

    return {
        .operands = operands,
        .operations = operations,
        .inputIndexes = inputIndexes,
        .outputIndexes = outputIndexes,
        .operandValues = operandValues,
        .pools = pools,
        .relaxComputationFloat32toFloat16 = true,
    };
}

inline bool is_ignored_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

// Create the model
Model createTestModel_dynamic_output_shape() {
    const std::vector<Operand> operands = {
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {2, 2},
            .numberOfConsumers = 1,
            .scale = 0.0078125f,
            .zeroPoint = 128,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 2},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 2},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 2},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 2},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 4},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 4},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 4},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 4},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_INT32,
            .dimensions = {4},
            .numberOfConsumers = 1,
            .scale = 3.1876640625e-05f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_INT32,
            .dimensions = {4},
            .numberOfConsumers = 1,
            .scale = 3.1876640625e-05f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_INT32,
            .dimensions = {4},
            .numberOfConsumers = 1,
            .scale = 3.1876640625e-05f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_INT32,
            .dimensions = {4},
            .numberOfConsumers = 1,
            .scale = 3.1876640625e-05f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT16_SYMM,
            .dimensions = {2, 4},
            .numberOfConsumers = 1,
            .scale = 0.00048828125f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {2, 4},
            .numberOfConsumers = 1,
            .scale = 0.0078125f,
            .zeroPoint = 128,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT16_SYMM,
            .dimensions = {0, 0},
            .numberOfConsumers = 0,
            .scale = 0.00048828125f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_OUTPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {0, 0},
            .numberOfConsumers = 0,
            .scale = 0.0078125f,
            .zeroPoint = 128,
            .lifetime = OperandLifeTime::MODEL_OUTPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        }
    };

    const std::vector<Operation> operations = {
        {
            .type = OperationType::QUANTIZED_16BIT_LSTM,
            .inputs = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14},
            .outputs = {15, 16},
        }
    };

    const std::vector<uint32_t> inputIndexes = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    const std::vector<uint32_t> outputIndexes = {15, 16};
    std::vector<uint8_t> operandValues = {};
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

// Create the model
Model createTestModel_dynamic_output_shape_relaxed() {
    const std::vector<Operand> operands = {
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {2, 2},
            .numberOfConsumers = 1,
            .scale = 0.0078125f,
            .zeroPoint = 128,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 2},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 2},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 2},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 2},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 4},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 4},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 4},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 4},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_INT32,
            .dimensions = {4},
            .numberOfConsumers = 1,
            .scale = 3.1876640625e-05f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_INT32,
            .dimensions = {4},
            .numberOfConsumers = 1,
            .scale = 3.1876640625e-05f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_INT32,
            .dimensions = {4},
            .numberOfConsumers = 1,
            .scale = 3.1876640625e-05f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_INT32,
            .dimensions = {4},
            .numberOfConsumers = 1,
            .scale = 3.1876640625e-05f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT16_SYMM,
            .dimensions = {2, 4},
            .numberOfConsumers = 1,
            .scale = 0.00048828125f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {2, 4},
            .numberOfConsumers = 1,
            .scale = 0.0078125f,
            .zeroPoint = 128,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT16_SYMM,
            .dimensions = {0, 0},
            .numberOfConsumers = 0,
            .scale = 0.00048828125f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_OUTPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {0, 0},
            .numberOfConsumers = 0,
            .scale = 0.0078125f,
            .zeroPoint = 128,
            .lifetime = OperandLifeTime::MODEL_OUTPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        }
    };

    const std::vector<Operation> operations = {
        {
            .type = OperationType::QUANTIZED_16BIT_LSTM,
            .inputs = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14},
            .outputs = {15, 16},
        }
    };

    const std::vector<uint32_t> inputIndexes = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    const std::vector<uint32_t> outputIndexes = {15, 16};
    std::vector<uint8_t> operandValues = {};
    const std::vector<hidl_memory> pools = {};

    return {
        .operands = operands,
        .operations = operations,
        .inputIndexes = inputIndexes,
        .outputIndexes = outputIndexes,
        .operandValues = operandValues,
        .pools = pools,
        .relaxComputationFloat32toFloat16 = true,
    };
}

inline bool is_ignored_dynamic_output_shape_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

// Create the model
Model createTestModel_2() {
    const std::vector<Operand> operands = {
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {1, 2},
            .numberOfConsumers = 1,
            .scale = 0.0078125f,
            .zeroPoint = 128,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 2},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 0, .length = 8},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 2},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 8, .length = 8},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 2},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 16, .length = 8},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 2},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 24, .length = 8},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 4},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 32, .length = 16},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 4},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 48, .length = 16},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 4},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 64, .length = 16},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 4},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 80, .length = 16},
        },
        {
            .type = OperandType::TENSOR_INT32,
            .dimensions = {4},
            .numberOfConsumers = 1,
            .scale = 3.1876640625e-05f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 96, .length = 16},
        },
        {
            .type = OperandType::TENSOR_INT32,
            .dimensions = {4},
            .numberOfConsumers = 1,
            .scale = 3.1876640625e-05f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 112, .length = 16},
        },
        {
            .type = OperandType::TENSOR_INT32,
            .dimensions = {4},
            .numberOfConsumers = 1,
            .scale = 3.1876640625e-05f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 128, .length = 16},
        },
        {
            .type = OperandType::TENSOR_INT32,
            .dimensions = {4},
            .numberOfConsumers = 1,
            .scale = 3.1876640625e-05f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 144, .length = 16},
        },
        {
            .type = OperandType::TENSOR_QUANT16_SYMM,
            .dimensions = {1, 4},
            .numberOfConsumers = 1,
            .scale = 0.00048828125f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {1, 4},
            .numberOfConsumers = 1,
            .scale = 0.0078125f,
            .zeroPoint = 128,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT16_SYMM,
            .dimensions = {1, 4},
            .numberOfConsumers = 0,
            .scale = 0.00048828125f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_OUTPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {1, 4},
            .numberOfConsumers = 0,
            .scale = 0.0078125f,
            .zeroPoint = 128,
            .lifetime = OperandLifeTime::MODEL_OUTPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        }
    };

    const std::vector<Operation> operations = {
        {
            .type = OperationType::QUANTIZED_16BIT_LSTM,
            .inputs = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14},
            .outputs = {15, 16},
        }
    };

    const std::vector<uint32_t> inputIndexes = {0, 13, 14};
    const std::vector<uint32_t> outputIndexes = {15, 16};
    std::vector<uint8_t> operandValues = {
      146, 250, 235, 171, 10, 218, 171, 108, 24, 50, 132, 179, 158, 110, 3, 169, 133, 34, 29, 49, 206, 109, 54, 183, 195, 187, 11, 99, 109, 10, 218, 48, 254, 206, 77, 168, 71, 20, 215, 6, 223, 7, 118, 225, 59, 130, 174, 26, 137, 240, 103, 52, 68, 51, 237, 112, 0, 220, 89, 23, 69, 4, 207, 253, 172, 60, 205, 65, 14, 0, 140, 168, 240, 223, 133, 56, 142, 64, 246, 216, 106, 214, 67, 23, 59, 158, 45, 3, 119, 132, 49, 205, 129, 218, 11, 98, 60, 225, 255, 255, 176, 52, 0, 0, 42, 253, 255, 255, 71, 128, 0, 0, 246, 35, 0, 0, 220, 72, 255, 255, 83, 210, 255, 255, 188, 104, 255, 255, 57, 154, 0, 0, 240, 189, 0, 0, 80, 191, 0, 0, 85, 172, 255, 255, 137, 25, 255, 255, 102, 189, 255, 255, 132, 92, 255, 255, 166, 97, 255, 255
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

inline bool is_ignored_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

// Create the model
Model createTestModel_relaxed_2() {
    const std::vector<Operand> operands = {
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {1, 2},
            .numberOfConsumers = 1,
            .scale = 0.0078125f,
            .zeroPoint = 128,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 2},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 0, .length = 8},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 2},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 8, .length = 8},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 2},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 16, .length = 8},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 2},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 24, .length = 8},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 4},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 32, .length = 16},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 4},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 48, .length = 16},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 4},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 64, .length = 16},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 4},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 80, .length = 16},
        },
        {
            .type = OperandType::TENSOR_INT32,
            .dimensions = {4},
            .numberOfConsumers = 1,
            .scale = 3.1876640625e-05f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 96, .length = 16},
        },
        {
            .type = OperandType::TENSOR_INT32,
            .dimensions = {4},
            .numberOfConsumers = 1,
            .scale = 3.1876640625e-05f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 112, .length = 16},
        },
        {
            .type = OperandType::TENSOR_INT32,
            .dimensions = {4},
            .numberOfConsumers = 1,
            .scale = 3.1876640625e-05f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 128, .length = 16},
        },
        {
            .type = OperandType::TENSOR_INT32,
            .dimensions = {4},
            .numberOfConsumers = 1,
            .scale = 3.1876640625e-05f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 144, .length = 16},
        },
        {
            .type = OperandType::TENSOR_QUANT16_SYMM,
            .dimensions = {1, 4},
            .numberOfConsumers = 1,
            .scale = 0.00048828125f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {1, 4},
            .numberOfConsumers = 1,
            .scale = 0.0078125f,
            .zeroPoint = 128,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT16_SYMM,
            .dimensions = {1, 4},
            .numberOfConsumers = 0,
            .scale = 0.00048828125f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_OUTPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {1, 4},
            .numberOfConsumers = 0,
            .scale = 0.0078125f,
            .zeroPoint = 128,
            .lifetime = OperandLifeTime::MODEL_OUTPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        }
    };

    const std::vector<Operation> operations = {
        {
            .type = OperationType::QUANTIZED_16BIT_LSTM,
            .inputs = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14},
            .outputs = {15, 16},
        }
    };

    const std::vector<uint32_t> inputIndexes = {0, 13, 14};
    const std::vector<uint32_t> outputIndexes = {15, 16};
    std::vector<uint8_t> operandValues = {
      146, 250, 235, 171, 10, 218, 171, 108, 24, 50, 132, 179, 158, 110, 3, 169, 133, 34, 29, 49, 206, 109, 54, 183, 195, 187, 11, 99, 109, 10, 218, 48, 254, 206, 77, 168, 71, 20, 215, 6, 223, 7, 118, 225, 59, 130, 174, 26, 137, 240, 103, 52, 68, 51, 237, 112, 0, 220, 89, 23, 69, 4, 207, 253, 172, 60, 205, 65, 14, 0, 140, 168, 240, 223, 133, 56, 142, 64, 246, 216, 106, 214, 67, 23, 59, 158, 45, 3, 119, 132, 49, 205, 129, 218, 11, 98, 60, 225, 255, 255, 176, 52, 0, 0, 42, 253, 255, 255, 71, 128, 0, 0, 246, 35, 0, 0, 220, 72, 255, 255, 83, 210, 255, 255, 188, 104, 255, 255, 57, 154, 0, 0, 240, 189, 0, 0, 80, 191, 0, 0, 85, 172, 255, 255, 137, 25, 255, 255, 102, 189, 255, 255, 132, 92, 255, 255, 166, 97, 255, 255
    };
    const std::vector<hidl_memory> pools = {};

    return {
        .operands = operands,
        .operations = operations,
        .inputIndexes = inputIndexes,
        .outputIndexes = outputIndexes,
        .operandValues = operandValues,
        .pools = pools,
        .relaxComputationFloat32toFloat16 = true,
    };
}

inline bool is_ignored_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

// Create the model
Model createTestModel_dynamic_output_shape_2() {
    const std::vector<Operand> operands = {
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {1, 2},
            .numberOfConsumers = 1,
            .scale = 0.0078125f,
            .zeroPoint = 128,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 2},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 0, .length = 8},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 2},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 8, .length = 8},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 2},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 16, .length = 8},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 2},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 24, .length = 8},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 4},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 32, .length = 16},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 4},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 48, .length = 16},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 4},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 64, .length = 16},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 4},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 80, .length = 16},
        },
        {
            .type = OperandType::TENSOR_INT32,
            .dimensions = {4},
            .numberOfConsumers = 1,
            .scale = 3.1876640625e-05f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 96, .length = 16},
        },
        {
            .type = OperandType::TENSOR_INT32,
            .dimensions = {4},
            .numberOfConsumers = 1,
            .scale = 3.1876640625e-05f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 112, .length = 16},
        },
        {
            .type = OperandType::TENSOR_INT32,
            .dimensions = {4},
            .numberOfConsumers = 1,
            .scale = 3.1876640625e-05f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 128, .length = 16},
        },
        {
            .type = OperandType::TENSOR_INT32,
            .dimensions = {4},
            .numberOfConsumers = 1,
            .scale = 3.1876640625e-05f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 144, .length = 16},
        },
        {
            .type = OperandType::TENSOR_QUANT16_SYMM,
            .dimensions = {1, 4},
            .numberOfConsumers = 1,
            .scale = 0.00048828125f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {1, 4},
            .numberOfConsumers = 1,
            .scale = 0.0078125f,
            .zeroPoint = 128,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT16_SYMM,
            .dimensions = {0, 0},
            .numberOfConsumers = 0,
            .scale = 0.00048828125f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_OUTPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {0, 0},
            .numberOfConsumers = 0,
            .scale = 0.0078125f,
            .zeroPoint = 128,
            .lifetime = OperandLifeTime::MODEL_OUTPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        }
    };

    const std::vector<Operation> operations = {
        {
            .type = OperationType::QUANTIZED_16BIT_LSTM,
            .inputs = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14},
            .outputs = {15, 16},
        }
    };

    const std::vector<uint32_t> inputIndexes = {0, 13, 14};
    const std::vector<uint32_t> outputIndexes = {15, 16};
    std::vector<uint8_t> operandValues = {
      146, 250, 235, 171, 10, 218, 171, 108, 24, 50, 132, 179, 158, 110, 3, 169, 133, 34, 29, 49, 206, 109, 54, 183, 195, 187, 11, 99, 109, 10, 218, 48, 254, 206, 77, 168, 71, 20, 215, 6, 223, 7, 118, 225, 59, 130, 174, 26, 137, 240, 103, 52, 68, 51, 237, 112, 0, 220, 89, 23, 69, 4, 207, 253, 172, 60, 205, 65, 14, 0, 140, 168, 240, 223, 133, 56, 142, 64, 246, 216, 106, 214, 67, 23, 59, 158, 45, 3, 119, 132, 49, 205, 129, 218, 11, 98, 60, 225, 255, 255, 176, 52, 0, 0, 42, 253, 255, 255, 71, 128, 0, 0, 246, 35, 0, 0, 220, 72, 255, 255, 83, 210, 255, 255, 188, 104, 255, 255, 57, 154, 0, 0, 240, 189, 0, 0, 80, 191, 0, 0, 85, 172, 255, 255, 137, 25, 255, 255, 102, 189, 255, 255, 132, 92, 255, 255, 166, 97, 255, 255
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

inline bool is_ignored_dynamic_output_shape_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

// Create the model
Model createTestModel_dynamic_output_shape_relaxed_2() {
    const std::vector<Operand> operands = {
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {1, 2},
            .numberOfConsumers = 1,
            .scale = 0.0078125f,
            .zeroPoint = 128,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 2},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 0, .length = 8},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 2},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 8, .length = 8},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 2},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 16, .length = 8},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 2},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 24, .length = 8},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 4},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 32, .length = 16},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 4},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 48, .length = 16},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 4},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 64, .length = 16},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {4, 4},
            .numberOfConsumers = 1,
            .scale = 0.00408021f,
            .zeroPoint = 100,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 80, .length = 16},
        },
        {
            .type = OperandType::TENSOR_INT32,
            .dimensions = {4},
            .numberOfConsumers = 1,
            .scale = 3.1876640625e-05f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 96, .length = 16},
        },
        {
            .type = OperandType::TENSOR_INT32,
            .dimensions = {4},
            .numberOfConsumers = 1,
            .scale = 3.1876640625e-05f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 112, .length = 16},
        },
        {
            .type = OperandType::TENSOR_INT32,
            .dimensions = {4},
            .numberOfConsumers = 1,
            .scale = 3.1876640625e-05f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 128, .length = 16},
        },
        {
            .type = OperandType::TENSOR_INT32,
            .dimensions = {4},
            .numberOfConsumers = 1,
            .scale = 3.1876640625e-05f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::CONSTANT_COPY,
            .location = {.poolIndex = 0, .offset = 144, .length = 16},
        },
        {
            .type = OperandType::TENSOR_QUANT16_SYMM,
            .dimensions = {1, 4},
            .numberOfConsumers = 1,
            .scale = 0.00048828125f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {1, 4},
            .numberOfConsumers = 1,
            .scale = 0.0078125f,
            .zeroPoint = 128,
            .lifetime = OperandLifeTime::MODEL_INPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT16_SYMM,
            .dimensions = {0, 0},
            .numberOfConsumers = 0,
            .scale = 0.00048828125f,
            .zeroPoint = 0,
            .lifetime = OperandLifeTime::MODEL_OUTPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        },
        {
            .type = OperandType::TENSOR_QUANT8_ASYMM,
            .dimensions = {0, 0},
            .numberOfConsumers = 0,
            .scale = 0.0078125f,
            .zeroPoint = 128,
            .lifetime = OperandLifeTime::MODEL_OUTPUT,
            .location = {.poolIndex = 0, .offset = 0, .length = 0},
        }
    };

    const std::vector<Operation> operations = {
        {
            .type = OperationType::QUANTIZED_16BIT_LSTM,
            .inputs = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14},
            .outputs = {15, 16},
        }
    };

    const std::vector<uint32_t> inputIndexes = {0, 13, 14};
    const std::vector<uint32_t> outputIndexes = {15, 16};
    std::vector<uint8_t> operandValues = {
      146, 250, 235, 171, 10, 218, 171, 108, 24, 50, 132, 179, 158, 110, 3, 169, 133, 34, 29, 49, 206, 109, 54, 183, 195, 187, 11, 99, 109, 10, 218, 48, 254, 206, 77, 168, 71, 20, 215, 6, 223, 7, 118, 225, 59, 130, 174, 26, 137, 240, 103, 52, 68, 51, 237, 112, 0, 220, 89, 23, 69, 4, 207, 253, 172, 60, 205, 65, 14, 0, 140, 168, 240, 223, 133, 56, 142, 64, 246, 216, 106, 214, 67, 23, 59, 158, 45, 3, 119, 132, 49, 205, 129, 218, 11, 98, 60, 225, 255, 255, 176, 52, 0, 0, 42, 253, 255, 255, 71, 128, 0, 0, 246, 35, 0, 0, 220, 72, 255, 255, 83, 210, 255, 255, 188, 104, 255, 255, 57, 154, 0, 0, 240, 189, 0, 0, 80, 191, 0, 0, 85, 172, 255, 255, 137, 25, 255, 255, 102, 189, 255, 255, 132, 92, 255, 255, 166, 97, 255, 255
    };
    const std::vector<hidl_memory> pools = {};

    return {
        .operands = operands,
        .operations = operations,
        .inputIndexes = inputIndexes,
        .outputIndexes = outputIndexes,
        .operandValues = operandValues,
        .pools = pools,
        .relaxComputationFloat32toFloat16 = true,
    };
}

inline bool is_ignored_dynamic_output_shape_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

