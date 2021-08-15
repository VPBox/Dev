// clang-format off
// Generated file (from: embedding_lookup_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace embedding_lookup_relaxed {
// Generated embedding_lookup_relaxed test
#include "generated/examples/embedding_lookup_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/embedding_lookup_relaxed.model.cpp"
} // namespace embedding_lookup_relaxed

TEST_F(GeneratedTests, embedding_lookup_relaxed) {
    execute(embedding_lookup_relaxed::CreateModel,
            embedding_lookup_relaxed::is_ignored,
            embedding_lookup_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, embedding_lookup_relaxed_dynamic_output_shape) {
    execute(embedding_lookup_relaxed::CreateModel_dynamic_output_shape,
            embedding_lookup_relaxed::is_ignored_dynamic_output_shape,
            embedding_lookup_relaxed::get_examples_dynamic_output_shape());
}

