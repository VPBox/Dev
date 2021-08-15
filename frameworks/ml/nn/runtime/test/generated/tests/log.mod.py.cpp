// clang-format off
// Generated file (from: log.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace log {
// Generated log test
#include "generated/examples/log.example.cpp"
// Generated model constructor
#include "generated/models/log.model.cpp"
} // namespace log

TEST_F(GeneratedTests, log) {
    execute(log::CreateModel,
            log::is_ignored,
            log::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, log, log::CreateModel)

TEST_F(GeneratedTests, log_relaxed) {
    execute(log::CreateModel_relaxed,
            log::is_ignored_relaxed,
            log::get_examples_relaxed());
}

TEST_F(GeneratedTests, log_float16) {
    execute(log::CreateModel_float16,
            log::is_ignored_float16,
            log::get_examples_float16());
}
TEST_AVAILABLE_SINCE(V1_2, log_float16, log::CreateModel_float16)

TEST_F(DynamicOutputShapeTest, log_dynamic_output_shape) {
    execute(log::CreateModel_dynamic_output_shape,
            log::is_ignored_dynamic_output_shape,
            log::get_examples_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, log_dynamic_output_shape_relaxed) {
    execute(log::CreateModel_dynamic_output_shape_relaxed,
            log::is_ignored_dynamic_output_shape_relaxed,
            log::get_examples_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, log_dynamic_output_shape_float16) {
    execute(log::CreateModel_dynamic_output_shape_float16,
            log::is_ignored_dynamic_output_shape_float16,
            log::get_examples_dynamic_output_shape_float16());
}

