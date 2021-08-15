// clang-format off
// Generated file (from: embedding_lookup.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace embedding_lookup {
// Generated embedding_lookup test
#include "generated/examples/embedding_lookup.example.cpp"
// Generated model constructor
#include "generated/models/embedding_lookup.model.cpp"
} // namespace embedding_lookup

TEST_F(GeneratedTests, embedding_lookup) {
    execute(embedding_lookup::CreateModel,
            embedding_lookup::is_ignored,
            embedding_lookup::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, embedding_lookup, embedding_lookup::CreateModel)

TEST_F(DynamicOutputShapeTest, embedding_lookup_dynamic_output_shape) {
    execute(embedding_lookup::CreateModel_dynamic_output_shape,
            embedding_lookup::is_ignored_dynamic_output_shape,
            embedding_lookup::get_examples_dynamic_output_shape());
}

