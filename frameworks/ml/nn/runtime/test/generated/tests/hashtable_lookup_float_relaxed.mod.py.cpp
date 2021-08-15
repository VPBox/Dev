// clang-format off
// Generated file (from: hashtable_lookup_float_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace hashtable_lookup_float_relaxed {
// Generated hashtable_lookup_float_relaxed test
#include "generated/examples/hashtable_lookup_float_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/hashtable_lookup_float_relaxed.model.cpp"
} // namespace hashtable_lookup_float_relaxed

TEST_F(GeneratedTests, hashtable_lookup_float_relaxed) {
    execute(hashtable_lookup_float_relaxed::CreateModel,
            hashtable_lookup_float_relaxed::is_ignored,
            hashtable_lookup_float_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, hashtable_lookup_float_relaxed_dynamic_output_shape) {
    execute(hashtable_lookup_float_relaxed::CreateModel_dynamic_output_shape,
            hashtable_lookup_float_relaxed::is_ignored_dynamic_output_shape,
            hashtable_lookup_float_relaxed::get_examples_dynamic_output_shape());
}

