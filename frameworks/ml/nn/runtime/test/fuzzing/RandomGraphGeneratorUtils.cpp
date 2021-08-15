/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "RandomGraphGeneratorUtils.h"

#include <algorithm>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>

#include "RandomGraphGenerator.h"
#include "RandomVariable.h"

namespace android {
namespace nn {
namespace fuzzing_test {

std::mt19937 RandomNumberGenerator::generator;

std::string Logger::getElapsedTime() {
    auto end = std::chrono::high_resolution_clock::now();
    int ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - mStart).count();
    int hour = ms / 3600000;
    int minutes = (ms % 3600000) / 60000;
    int seconds = (ms % 60000) / 1000;
    int milli = ms % 1000;
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << hour << ":" << std::setw(2) << minutes << ":"
        << std::setw(2) << seconds << "." << std::setw(3) << milli << " ";
    return oss.str();
}

SpecWriter::SpecWriter(std::string filename, std::string testname) : os(filename) {
    if (os.is_open() && testname != "") {
        os << "# Generated from " << testname << ". Do not edit.\n\n";
    }
}

// Main entrance of dumping the spec file.
// Check nn/tools/test_generator/README.md for guide on spec file syntax.
void SpecWriter::dump(const std::vector<RandomOperation>& operations,
                      const std::vector<std::shared_ptr<RandomOperand>>& operands) {
    // RandomGraphGenerator does not support dynamic output shape.
    os << "Configuration.test_dynamic_output_shape = False\n\n";

    // Dump model operands.
    os << "# Model operands\n";
    for (auto& operand : operands) dump(operand);

    // Dump model operations.
    os << "\n# Model operations\nmodel = Model()\n";
    for (const auto& operation : operations) dump(operation);

    // Dump input/output buffers.
    os << "\n# Example\n";
    os << "Example({\n";
    for (auto& operand : operands) {
        if (operand->type == RandomOperandType::CONST ||
            operand->type == RandomOperandType::INTERNAL) {
            continue;
        }
        os << "    op" << operand->opIndex << ": [";
        dump(operand->dataType, reinterpret_cast<uint8_t*>(operand->buffer.data()),
             operand->getNumberOfElements());
        os << "],\n";
    }
    os << "})\n";
}

// Dump an operand buffer.
void SpecWriter::dump(test_wrapper::Type type, const uint8_t* buffer, uint32_t length) {
    if (buffer == nullptr) return;
    switch (type) {
        case test_wrapper::Type::FLOAT32:
        case test_wrapper::Type::TENSOR_FLOAT32:
            dump(reinterpret_cast<const float*>(buffer), length);
            break;
        case test_wrapper::Type::INT32:
        case test_wrapper::Type::TENSOR_INT32:
            dump(reinterpret_cast<const int32_t*>(buffer), length);
            break;
        case test_wrapper::Type::TENSOR_QUANT8_ASYMM:
            dump(reinterpret_cast<const uint8_t*>(buffer), length);
            break;
        case test_wrapper::Type::TENSOR_QUANT8_SYMM:
            dump(reinterpret_cast<const int8_t*>(buffer), length);
            break;
        case test_wrapper::Type::TENSOR_QUANT16_ASYMM:
            dump(reinterpret_cast<const uint16_t*>(buffer), length);
            break;
        case test_wrapper::Type::TENSOR_QUANT16_SYMM:
            dump(reinterpret_cast<const int16_t*>(buffer), length);
            break;
        case test_wrapper::Type::BOOL:
        case test_wrapper::Type::TENSOR_BOOL8:
            dump(reinterpret_cast<const bool8*>(buffer), length);
            break;
        case test_wrapper::Type::FLOAT16:
        case test_wrapper::Type::TENSOR_FLOAT16:
            dump(reinterpret_cast<const _Float16*>(buffer), length);
            break;
        default:
            NN_FUZZER_CHECK(false) << "Unknown type when dumping the buffer";
    }
}

// Dump dimensions with curly braces.
void SpecWriter::dump(const std::vector<RandomVariable>& dimensions) {
    os << "{" << joinStr(", ", dimensions, [](const RandomVariable& var) {
        return std::to_string(var.getValue());
    }) << "}";
}

// Dump a model operand.
// e.g. op0 = Input("op0", "TENSOR_FLOAT32", "{1, 2, 6, 1}")
// e.g. op1 = Parameter("op1", "INT32", "{}", [2])
void SpecWriter::dump(const std::shared_ptr<RandomOperand>& op) {
    os << "op" << op->opIndex << " = " << toString(op->type) << "(\"op" << op->opIndex << "\", \""
       << toString(op->dataType) << "\", \"";
    dump(op->dimensions);
    if (op->scale != 0.0f || op->zeroPoint != 0) {
        os << ", " << op->scale << "f, " << op->zeroPoint;
    }
    os << "\"";
    if (op->type == RandomOperandType::CONST) {
        os << ", [";
        dump(op->dataType, reinterpret_cast<uint8_t*>(op->buffer.data()),
             op->getNumberOfElements());
        os << "]";
    }
    os << ")\n";
}

// Dump a model operation.
// e.g. model = model.Operation("CONV_2D", op0, op1, op2, op3, op4, op5, op6).To(op7)
void SpecWriter::dump(const RandomOperation& op) {
    auto getOperandStr = [](std::shared_ptr<RandomOperand> op) {
        return "op" + std::to_string(op->opIndex);
    };
    os << "model = model.Operation(\"" << kOperationNames[op.opType] << "\", "
       << joinStr(", ", op.inputs, getOperandStr) << ").To("
       << joinStr(", ", op.outputs, getOperandStr) << ")\n";
}

}  // namespace fuzzing_test
}  // namespace nn
}  // namespace android
