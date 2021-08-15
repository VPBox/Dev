// clang-format off
// Generated file (from: gather.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace gather {
// Generated gather test
#include "generated/examples/gather.example.cpp"
// Generated model constructor
#include "generated/models/gather.model.cpp"
} // namespace gather

TEST_F(GeneratedTests, gather) {
    execute(gather::CreateModel,
            gather::is_ignored,
            gather::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, gather, gather::CreateModel)

TEST_F(GeneratedTests, gather_relaxed) {
    execute(gather::CreateModel_relaxed,
            gather::is_ignored_relaxed,
            gather::get_examples_relaxed());
}

TEST_F(GeneratedTests, gather_quant8) {
    execute(gather::CreateModel_quant8,
            gather::is_ignored_quant8,
            gather::get_examples_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, gather_quant8, gather::CreateModel_quant8)

TEST_F(GeneratedTests, gather_int32) {
    execute(gather::CreateModel_int32,
            gather::is_ignored_int32,
            gather::get_examples_int32());
}
TEST_AVAILABLE_SINCE(V1_2, gather_int32, gather::CreateModel_int32)

TEST_F(GeneratedTests, gather_float16) {
    execute(gather::CreateModel_float16,
            gather::is_ignored_float16,
            gather::get_examples_float16());
}
TEST_AVAILABLE_SINCE(V1_2, gather_float16, gather::CreateModel_float16)

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape) {
    execute(gather::CreateModel_dynamic_output_shape,
            gather::is_ignored_dynamic_output_shape,
            gather::get_examples_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_relaxed) {
    execute(gather::CreateModel_dynamic_output_shape_relaxed,
            gather::is_ignored_dynamic_output_shape_relaxed,
            gather::get_examples_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_quant8) {
    execute(gather::CreateModel_dynamic_output_shape_quant8,
            gather::is_ignored_dynamic_output_shape_quant8,
            gather::get_examples_dynamic_output_shape_quant8());
}

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_int32) {
    execute(gather::CreateModel_dynamic_output_shape_int32,
            gather::is_ignored_dynamic_output_shape_int32,
            gather::get_examples_dynamic_output_shape_int32());
}

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_float16) {
    execute(gather::CreateModel_dynamic_output_shape_float16,
            gather::is_ignored_dynamic_output_shape_float16,
            gather::get_examples_dynamic_output_shape_float16());
}

TEST_F(GeneratedTests, gather_2) {
    execute(gather::CreateModel_2,
            gather::is_ignored_2,
            gather::get_examples_2());
}
TEST_AVAILABLE_SINCE(V1_2, gather_2, gather::CreateModel_2)

TEST_F(GeneratedTests, gather_relaxed_2) {
    execute(gather::CreateModel_relaxed_2,
            gather::is_ignored_relaxed_2,
            gather::get_examples_relaxed_2());
}

TEST_F(GeneratedTests, gather_quant8_2) {
    execute(gather::CreateModel_quant8_2,
            gather::is_ignored_quant8_2,
            gather::get_examples_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, gather_quant8_2, gather::CreateModel_quant8_2)

TEST_F(GeneratedTests, gather_int32_2) {
    execute(gather::CreateModel_int32_2,
            gather::is_ignored_int32_2,
            gather::get_examples_int32_2());
}
TEST_AVAILABLE_SINCE(V1_2, gather_int32_2, gather::CreateModel_int32_2)

TEST_F(GeneratedTests, gather_float16_2) {
    execute(gather::CreateModel_float16_2,
            gather::is_ignored_float16_2,
            gather::get_examples_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, gather_float16_2, gather::CreateModel_float16_2)

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_2) {
    execute(gather::CreateModel_dynamic_output_shape_2,
            gather::is_ignored_dynamic_output_shape_2,
            gather::get_examples_dynamic_output_shape_2());
}

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_relaxed_2) {
    execute(gather::CreateModel_dynamic_output_shape_relaxed_2,
            gather::is_ignored_dynamic_output_shape_relaxed_2,
            gather::get_examples_dynamic_output_shape_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_quant8_2) {
    execute(gather::CreateModel_dynamic_output_shape_quant8_2,
            gather::is_ignored_dynamic_output_shape_quant8_2,
            gather::get_examples_dynamic_output_shape_quant8_2());
}

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_int32_2) {
    execute(gather::CreateModel_dynamic_output_shape_int32_2,
            gather::is_ignored_dynamic_output_shape_int32_2,
            gather::get_examples_dynamic_output_shape_int32_2());
}

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_float16_2) {
    execute(gather::CreateModel_dynamic_output_shape_float16_2,
            gather::is_ignored_dynamic_output_shape_float16_2,
            gather::get_examples_dynamic_output_shape_float16_2());
}

TEST_F(GeneratedTests, gather_3) {
    execute(gather::CreateModel_3,
            gather::is_ignored_3,
            gather::get_examples_3());
}
TEST_AVAILABLE_SINCE(V1_2, gather_3, gather::CreateModel_3)

TEST_F(GeneratedTests, gather_relaxed_3) {
    execute(gather::CreateModel_relaxed_3,
            gather::is_ignored_relaxed_3,
            gather::get_examples_relaxed_3());
}

TEST_F(GeneratedTests, gather_quant8_3) {
    execute(gather::CreateModel_quant8_3,
            gather::is_ignored_quant8_3,
            gather::get_examples_quant8_3());
}
TEST_AVAILABLE_SINCE(V1_2, gather_quant8_3, gather::CreateModel_quant8_3)

TEST_F(GeneratedTests, gather_int32_3) {
    execute(gather::CreateModel_int32_3,
            gather::is_ignored_int32_3,
            gather::get_examples_int32_3());
}
TEST_AVAILABLE_SINCE(V1_2, gather_int32_3, gather::CreateModel_int32_3)

TEST_F(GeneratedTests, gather_float16_3) {
    execute(gather::CreateModel_float16_3,
            gather::is_ignored_float16_3,
            gather::get_examples_float16_3());
}
TEST_AVAILABLE_SINCE(V1_2, gather_float16_3, gather::CreateModel_float16_3)

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_3) {
    execute(gather::CreateModel_dynamic_output_shape_3,
            gather::is_ignored_dynamic_output_shape_3,
            gather::get_examples_dynamic_output_shape_3());
}

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_relaxed_3) {
    execute(gather::CreateModel_dynamic_output_shape_relaxed_3,
            gather::is_ignored_dynamic_output_shape_relaxed_3,
            gather::get_examples_dynamic_output_shape_relaxed_3());
}

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_quant8_3) {
    execute(gather::CreateModel_dynamic_output_shape_quant8_3,
            gather::is_ignored_dynamic_output_shape_quant8_3,
            gather::get_examples_dynamic_output_shape_quant8_3());
}

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_int32_3) {
    execute(gather::CreateModel_dynamic_output_shape_int32_3,
            gather::is_ignored_dynamic_output_shape_int32_3,
            gather::get_examples_dynamic_output_shape_int32_3());
}

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_float16_3) {
    execute(gather::CreateModel_dynamic_output_shape_float16_3,
            gather::is_ignored_dynamic_output_shape_float16_3,
            gather::get_examples_dynamic_output_shape_float16_3());
}

TEST_F(GeneratedTests, gather_4) {
    execute(gather::CreateModel_4,
            gather::is_ignored_4,
            gather::get_examples_4());
}
TEST_AVAILABLE_SINCE(V1_2, gather_4, gather::CreateModel_4)

TEST_F(GeneratedTests, gather_relaxed_4) {
    execute(gather::CreateModel_relaxed_4,
            gather::is_ignored_relaxed_4,
            gather::get_examples_relaxed_4());
}

TEST_F(GeneratedTests, gather_quant8_4) {
    execute(gather::CreateModel_quant8_4,
            gather::is_ignored_quant8_4,
            gather::get_examples_quant8_4());
}
TEST_AVAILABLE_SINCE(V1_2, gather_quant8_4, gather::CreateModel_quant8_4)

TEST_F(GeneratedTests, gather_int32_4) {
    execute(gather::CreateModel_int32_4,
            gather::is_ignored_int32_4,
            gather::get_examples_int32_4());
}
TEST_AVAILABLE_SINCE(V1_2, gather_int32_4, gather::CreateModel_int32_4)

TEST_F(GeneratedTests, gather_float16_4) {
    execute(gather::CreateModel_float16_4,
            gather::is_ignored_float16_4,
            gather::get_examples_float16_4());
}
TEST_AVAILABLE_SINCE(V1_2, gather_float16_4, gather::CreateModel_float16_4)

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_4) {
    execute(gather::CreateModel_dynamic_output_shape_4,
            gather::is_ignored_dynamic_output_shape_4,
            gather::get_examples_dynamic_output_shape_4());
}

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_relaxed_4) {
    execute(gather::CreateModel_dynamic_output_shape_relaxed_4,
            gather::is_ignored_dynamic_output_shape_relaxed_4,
            gather::get_examples_dynamic_output_shape_relaxed_4());
}

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_quant8_4) {
    execute(gather::CreateModel_dynamic_output_shape_quant8_4,
            gather::is_ignored_dynamic_output_shape_quant8_4,
            gather::get_examples_dynamic_output_shape_quant8_4());
}

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_int32_4) {
    execute(gather::CreateModel_dynamic_output_shape_int32_4,
            gather::is_ignored_dynamic_output_shape_int32_4,
            gather::get_examples_dynamic_output_shape_int32_4());
}

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_float16_4) {
    execute(gather::CreateModel_dynamic_output_shape_float16_4,
            gather::is_ignored_dynamic_output_shape_float16_4,
            gather::get_examples_dynamic_output_shape_float16_4());
}

TEST_F(GeneratedTests, gather_5) {
    execute(gather::CreateModel_5,
            gather::is_ignored_5,
            gather::get_examples_5());
}
TEST_AVAILABLE_SINCE(V1_2, gather_5, gather::CreateModel_5)

TEST_F(GeneratedTests, gather_relaxed_5) {
    execute(gather::CreateModel_relaxed_5,
            gather::is_ignored_relaxed_5,
            gather::get_examples_relaxed_5());
}

TEST_F(GeneratedTests, gather_quant8_5) {
    execute(gather::CreateModel_quant8_5,
            gather::is_ignored_quant8_5,
            gather::get_examples_quant8_5());
}
TEST_AVAILABLE_SINCE(V1_2, gather_quant8_5, gather::CreateModel_quant8_5)

TEST_F(GeneratedTests, gather_int32_5) {
    execute(gather::CreateModel_int32_5,
            gather::is_ignored_int32_5,
            gather::get_examples_int32_5());
}
TEST_AVAILABLE_SINCE(V1_2, gather_int32_5, gather::CreateModel_int32_5)

TEST_F(GeneratedTests, gather_float16_5) {
    execute(gather::CreateModel_float16_5,
            gather::is_ignored_float16_5,
            gather::get_examples_float16_5());
}
TEST_AVAILABLE_SINCE(V1_2, gather_float16_5, gather::CreateModel_float16_5)

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_5) {
    execute(gather::CreateModel_dynamic_output_shape_5,
            gather::is_ignored_dynamic_output_shape_5,
            gather::get_examples_dynamic_output_shape_5());
}

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_relaxed_5) {
    execute(gather::CreateModel_dynamic_output_shape_relaxed_5,
            gather::is_ignored_dynamic_output_shape_relaxed_5,
            gather::get_examples_dynamic_output_shape_relaxed_5());
}

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_quant8_5) {
    execute(gather::CreateModel_dynamic_output_shape_quant8_5,
            gather::is_ignored_dynamic_output_shape_quant8_5,
            gather::get_examples_dynamic_output_shape_quant8_5());
}

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_int32_5) {
    execute(gather::CreateModel_dynamic_output_shape_int32_5,
            gather::is_ignored_dynamic_output_shape_int32_5,
            gather::get_examples_dynamic_output_shape_int32_5());
}

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_float16_5) {
    execute(gather::CreateModel_dynamic_output_shape_float16_5,
            gather::is_ignored_dynamic_output_shape_float16_5,
            gather::get_examples_dynamic_output_shape_float16_5());
}

TEST_F(GeneratedTests, gather_6) {
    execute(gather::CreateModel_6,
            gather::is_ignored_6,
            gather::get_examples_6());
}
TEST_AVAILABLE_SINCE(V1_2, gather_6, gather::CreateModel_6)

TEST_F(GeneratedTests, gather_relaxed_6) {
    execute(gather::CreateModel_relaxed_6,
            gather::is_ignored_relaxed_6,
            gather::get_examples_relaxed_6());
}

TEST_F(GeneratedTests, gather_quant8_6) {
    execute(gather::CreateModel_quant8_6,
            gather::is_ignored_quant8_6,
            gather::get_examples_quant8_6());
}
TEST_AVAILABLE_SINCE(V1_2, gather_quant8_6, gather::CreateModel_quant8_6)

TEST_F(GeneratedTests, gather_int32_6) {
    execute(gather::CreateModel_int32_6,
            gather::is_ignored_int32_6,
            gather::get_examples_int32_6());
}
TEST_AVAILABLE_SINCE(V1_2, gather_int32_6, gather::CreateModel_int32_6)

TEST_F(GeneratedTests, gather_float16_6) {
    execute(gather::CreateModel_float16_6,
            gather::is_ignored_float16_6,
            gather::get_examples_float16_6());
}
TEST_AVAILABLE_SINCE(V1_2, gather_float16_6, gather::CreateModel_float16_6)

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_6) {
    execute(gather::CreateModel_dynamic_output_shape_6,
            gather::is_ignored_dynamic_output_shape_6,
            gather::get_examples_dynamic_output_shape_6());
}

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_relaxed_6) {
    execute(gather::CreateModel_dynamic_output_shape_relaxed_6,
            gather::is_ignored_dynamic_output_shape_relaxed_6,
            gather::get_examples_dynamic_output_shape_relaxed_6());
}

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_quant8_6) {
    execute(gather::CreateModel_dynamic_output_shape_quant8_6,
            gather::is_ignored_dynamic_output_shape_quant8_6,
            gather::get_examples_dynamic_output_shape_quant8_6());
}

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_int32_6) {
    execute(gather::CreateModel_dynamic_output_shape_int32_6,
            gather::is_ignored_dynamic_output_shape_int32_6,
            gather::get_examples_dynamic_output_shape_int32_6());
}

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_float16_6) {
    execute(gather::CreateModel_dynamic_output_shape_float16_6,
            gather::is_ignored_dynamic_output_shape_float16_6,
            gather::get_examples_dynamic_output_shape_float16_6());
}

TEST_F(GeneratedTests, gather_7) {
    execute(gather::CreateModel_7,
            gather::is_ignored_7,
            gather::get_examples_7());
}
TEST_AVAILABLE_SINCE(V1_2, gather_7, gather::CreateModel_7)

TEST_F(GeneratedTests, gather_relaxed_7) {
    execute(gather::CreateModel_relaxed_7,
            gather::is_ignored_relaxed_7,
            gather::get_examples_relaxed_7());
}

TEST_F(GeneratedTests, gather_quant8_7) {
    execute(gather::CreateModel_quant8_7,
            gather::is_ignored_quant8_7,
            gather::get_examples_quant8_7());
}
TEST_AVAILABLE_SINCE(V1_2, gather_quant8_7, gather::CreateModel_quant8_7)

TEST_F(GeneratedTests, gather_int32_7) {
    execute(gather::CreateModel_int32_7,
            gather::is_ignored_int32_7,
            gather::get_examples_int32_7());
}
TEST_AVAILABLE_SINCE(V1_2, gather_int32_7, gather::CreateModel_int32_7)

TEST_F(GeneratedTests, gather_float16_7) {
    execute(gather::CreateModel_float16_7,
            gather::is_ignored_float16_7,
            gather::get_examples_float16_7());
}
TEST_AVAILABLE_SINCE(V1_2, gather_float16_7, gather::CreateModel_float16_7)

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_7) {
    execute(gather::CreateModel_dynamic_output_shape_7,
            gather::is_ignored_dynamic_output_shape_7,
            gather::get_examples_dynamic_output_shape_7());
}

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_relaxed_7) {
    execute(gather::CreateModel_dynamic_output_shape_relaxed_7,
            gather::is_ignored_dynamic_output_shape_relaxed_7,
            gather::get_examples_dynamic_output_shape_relaxed_7());
}

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_quant8_7) {
    execute(gather::CreateModel_dynamic_output_shape_quant8_7,
            gather::is_ignored_dynamic_output_shape_quant8_7,
            gather::get_examples_dynamic_output_shape_quant8_7());
}

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_int32_7) {
    execute(gather::CreateModel_dynamic_output_shape_int32_7,
            gather::is_ignored_dynamic_output_shape_int32_7,
            gather::get_examples_dynamic_output_shape_int32_7());
}

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_float16_7) {
    execute(gather::CreateModel_dynamic_output_shape_float16_7,
            gather::is_ignored_dynamic_output_shape_float16_7,
            gather::get_examples_dynamic_output_shape_float16_7());
}

TEST_F(GeneratedTests, gather_8) {
    execute(gather::CreateModel_8,
            gather::is_ignored_8,
            gather::get_examples_8());
}
TEST_AVAILABLE_SINCE(V1_2, gather_8, gather::CreateModel_8)

TEST_F(GeneratedTests, gather_relaxed_8) {
    execute(gather::CreateModel_relaxed_8,
            gather::is_ignored_relaxed_8,
            gather::get_examples_relaxed_8());
}

TEST_F(GeneratedTests, gather_quant8_8) {
    execute(gather::CreateModel_quant8_8,
            gather::is_ignored_quant8_8,
            gather::get_examples_quant8_8());
}
TEST_AVAILABLE_SINCE(V1_2, gather_quant8_8, gather::CreateModel_quant8_8)

TEST_F(GeneratedTests, gather_int32_8) {
    execute(gather::CreateModel_int32_8,
            gather::is_ignored_int32_8,
            gather::get_examples_int32_8());
}
TEST_AVAILABLE_SINCE(V1_2, gather_int32_8, gather::CreateModel_int32_8)

TEST_F(GeneratedTests, gather_float16_8) {
    execute(gather::CreateModel_float16_8,
            gather::is_ignored_float16_8,
            gather::get_examples_float16_8());
}
TEST_AVAILABLE_SINCE(V1_2, gather_float16_8, gather::CreateModel_float16_8)

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_8) {
    execute(gather::CreateModel_dynamic_output_shape_8,
            gather::is_ignored_dynamic_output_shape_8,
            gather::get_examples_dynamic_output_shape_8());
}

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_relaxed_8) {
    execute(gather::CreateModel_dynamic_output_shape_relaxed_8,
            gather::is_ignored_dynamic_output_shape_relaxed_8,
            gather::get_examples_dynamic_output_shape_relaxed_8());
}

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_quant8_8) {
    execute(gather::CreateModel_dynamic_output_shape_quant8_8,
            gather::is_ignored_dynamic_output_shape_quant8_8,
            gather::get_examples_dynamic_output_shape_quant8_8());
}

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_int32_8) {
    execute(gather::CreateModel_dynamic_output_shape_int32_8,
            gather::is_ignored_dynamic_output_shape_int32_8,
            gather::get_examples_dynamic_output_shape_int32_8());
}

TEST_F(DynamicOutputShapeTest, gather_dynamic_output_shape_float16_8) {
    execute(gather::CreateModel_dynamic_output_shape_float16_8,
            gather::is_ignored_dynamic_output_shape_float16_8,
            gather::get_examples_dynamic_output_shape_float16_8());
}

