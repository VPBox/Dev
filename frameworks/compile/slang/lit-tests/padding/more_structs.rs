// RUN: %Slang %s

// RUN: %rs-filecheck-wrapper %s --check-prefix=CHECK-LL
//
//     Check that the data types are properly padded:
//
// CHECK-LL: %struct.char_struct{{(\.[0-9]+)?}} = type { i16, [6 x i8], i64 }
// CHECK-LL: %struct.five_struct{{(\.[0-9]+)?}} = type { i8, [7 x i8], i64, i16, [6 x i8], i64, half, [6 x i8] }
//
//     Check that the helper function for unpacking an invokable's arguments
//     accesses a properly padded struct:
//
// CHECK-LL: define void @.helper_check_char_struct({ i16, [6 x i8], i64 }* nocapture)
// CHECK-LL: [[C_F1_ADDR:%[0-9]+]] = getelementptr inbounds { i16, [6 x i8], i64 }, { i16, [6 x i8], i64 }* %0, i{{[0-9]+}} 0, i32 0
// CHECK-LL: [[C_F1_VAL:%[0-9]+]] = load i16, i16* [[C_F1_ADDR]]
// CHECK-LL: [[C_F2_ADDR:%[0-9]+]] = getelementptr inbounds { i16, [6 x i8], i64 }, { i16, [6 x i8], i64 }* %0, i{{[0-9]+}} 0, i32 2
// CHECK-LL: [[C_F2_VAL:%[0-9]+]] = load i64, i64* [[C_F2_ADDR]]
// CHECK-LL: tail call void @check_char_struct(i16 [[C_F1_VAL]], i64 [[C_F2_VAL]])
//
// CHECK-LL: define void @.helper_check_five_struct({ i8, [7 x i8], i64, i16, [6 x i8], i64, half, [6 x i8] }* nocapture)
// CHECK-LL: [[F_F1_ADDR:%[0-9]+]] = getelementptr inbounds { i8, [7 x i8], i64, i16, [6 x i8], i64, half, [6 x i8] }, { i8, [7 x i8], i64, i16, [6 x i8], i64, half, [6 x i8] }* %0, i{{[0-9]+}} 0, i32 0
// CHECK-LL: [[F_F1_VAL:%[0-9]+]] = load i8, i8* [[F_F1_ADDR]]
// CHECK-LL: [[F_F2_ADDR:%[0-9]+]] = getelementptr inbounds { i8, [7 x i8], i64, i16, [6 x i8], i64, half, [6 x i8] }, { i8, [7 x i8], i64, i16, [6 x i8], i64, half, [6 x i8] }* %0, i{{[0-9]+}} 0, i32 2
// CHECK-LL: [[F_F2_VAL:%[0-9]+]] = load i64, i64* [[F_F2_ADDR]]
// CHECK-LL: [[F_F3_ADDR:%[0-9]+]] = getelementptr inbounds { i8, [7 x i8], i64, i16, [6 x i8], i64, half, [6 x i8] }, { i8, [7 x i8], i64, i16, [6 x i8], i64, half, [6 x i8] }* %0, i{{[0-9]+}} 0, i32 3
// CHECK-LL: [[F_F3_VAL:%[0-9]+]] = load i16, i16* [[F_F3_ADDR]]
// CHECK-LL: [[F_F4_ADDR:%[0-9]+]] = getelementptr inbounds { i8, [7 x i8], i64, i16, [6 x i8], i64, half, [6 x i8] }, { i8, [7 x i8], i64, i16, [6 x i8], i64, half, [6 x i8] }* %0, i{{[0-9]+}} 0, i32 5
// CHECK-LL: [[F_F4_VAL:%[0-9]+]] = load i64, i64* [[F_F4_ADDR]]
// CHECK-LL: [[F_F5_ADDR:%[0-9]+]] = getelementptr inbounds { i8, [7 x i8], i64, i16, [6 x i8], i64, half, [6 x i8] }, { i8, [7 x i8], i64, i16, [6 x i8], i64, half, [6 x i8] }* %0, i{{[0-9]+}} 0, i32 6
// CHECK-LL: [[F_F5_VAL:%[0-9]+]] = load half, half* [[F_F5_ADDR]]
// CHECK-LL: tail call void @check_five_struct(i8 [[F_F1_VAL]], i64 [[F_F2_VAL]], i16 [[F_F3_VAL]], i64 [[F_F4_VAL]], half [[F_F5_VAL]])

// RUN: %scriptc-filecheck-wrapper --lang=Java --type=char_struct --check-prefix=CHECK-JAVA-CHAR-STRUCT %s
// CHECK-JAVA-CHAR-STRUCT:      public static Element createElement(RenderScript rs) {
// CHECK-JAVA-CHAR-STRUCT-NEXT:     Element.Builder eb = new Element.Builder(rs);
// CHECK-JAVA-CHAR-STRUCT-NEXT:     eb.add(Element.I16(rs), "f1");
// CHECK-JAVA-CHAR-STRUCT-NEXT:     eb.add(Element.U32(rs), "#rs_padding_1");
// CHECK-JAVA-CHAR-STRUCT-NEXT:     eb.add(Element.U16(rs), "#rs_padding_2");
// CHECK-JAVA-CHAR-STRUCT-NEXT:     eb.add(Element.I64(rs), "f2");
// CHECK-JAVA-CHAR-STRUCT-NEXT:     return eb.create();
// CHECK-JAVA-CHAR-STRUCT-NEXT: }

// RUN: %scriptc-filecheck-wrapper --lang=Java --type=five_struct --check-prefix=CHECK-JAVA-FIVE-STRUCT %s
// CHECK-JAVA-FIVE-STRUCT:      public static Element createElement(RenderScript rs) {
// CHECK-JAVA-FIVE-STRUCT-NEXT:     Element.Builder eb = new Element.Builder(rs);
// CHECK-JAVA-FIVE-STRUCT-NEXT:     eb.add(Element.I8(rs), "f1");
// CHECK-JAVA-FIVE-STRUCT-NEXT:     eb.add(Element.U32(rs), "#rs_padding_1");
// CHECK-JAVA-FIVE-STRUCT-NEXT:     eb.add(Element.U16(rs), "#rs_padding_2");
// CHECK-JAVA-FIVE-STRUCT-NEXT:     eb.add(Element.U8(rs), "#rs_padding_3");
// CHECK-JAVA-FIVE-STRUCT-NEXT:     eb.add(Element.I64(rs), "f2");
// CHECK-JAVA-FIVE-STRUCT-NEXT:     eb.add(Element.I16(rs), "f3");
// CHECK-JAVA-FIVE-STRUCT-NEXT:     eb.add(Element.U32(rs), "#rs_padding_4");
// CHECK-JAVA-FIVE-STRUCT-NEXT:     eb.add(Element.U16(rs), "#rs_padding_5");
// CHECK-JAVA-FIVE-STRUCT-NEXT:     eb.add(Element.I64(rs), "f4");
// CHECK-JAVA-FIVE-STRUCT-NEXT:     eb.add(Element.F16(rs), "f5");
// CHECK-JAVA-FIVE-STRUCT-NEXT:     eb.add(Element.U32(rs), "#rs_padding_6");
// CHECK-JAVA-FIVE-STRUCT-NEXT:     eb.add(Element.U16(rs), "#rs_padding_7");
// CHECK-JAVA-FIVE-STRUCT-NEXT:     return eb.create();
// CHECK-JAVA-FIVE-STRUCT-NEXT: }

// RUN: %scriptc-filecheck-wrapper --lang=Java --check-prefix=CHECK-JAVA-INVOKE %s
//
// CHECK-JAVA-INVOKE:      public void invoke_check_char_struct(short arg1, long arg2) {
// CHECK-JAVA-INVOKE-NEXT:     FieldPacker check_char_struct_fp = new FieldPacker(16);
// CHECK-JAVA-INVOKE-NEXT:     check_char_struct_fp.addI16(arg1);
// CHECK-JAVA-INVOKE-NEXT:     check_char_struct_fp.skip(6);
// CHECK-JAVA-INVOKE-NEXT:     check_char_struct_fp.addI64(arg2);
// CHECK-JAVA-INVOKE-NEXT:     invoke(mExportFuncIdx_check_char_struct, check_char_struct_fp);
// CHECK-JAVA-INVOKE-NEXT: }
//
// CHECK-JAVA-INVOKE:      public void invoke_check_five_struct(byte arg1, long arg2, short arg3, long arg4, short arg5) {
// CHECK-JAVA-INVOKE-NEXT:     FieldPacker check_five_struct_fp = new FieldPacker(40);
// CHECK-JAVA-INVOKE-NEXT:     check_five_struct_fp.addI8(arg1);
// CHECK-JAVA-INVOKE-NEXT:     check_five_struct_fp.skip(7);
// CHECK-JAVA-INVOKE-NEXT:     check_five_struct_fp.addI64(arg2);
// CHECK-JAVA-INVOKE-NEXT:     check_five_struct_fp.addI16(arg3);
// CHECK-JAVA-INVOKE-NEXT:     check_five_struct_fp.skip(6);
// CHECK-JAVA-INVOKE-NEXT:     check_five_struct_fp.addI64(arg4);
// CHECK-JAVA-INVOKE-NEXT:     check_five_struct_fp.addI16(arg5);
// CHECK-JAVA-INVOKE-NEXT:     check_five_struct_fp.skip(6);
// CHECK-JAVA-INVOKE-NEXT:     invoke(mExportFuncIdx_check_five_struct, check_five_struct_fp);
// CHECK-JAVA-INVOKE-NEXT: }

// Some more structs with different field types and/or more fields.

#pragma version(1)
#pragma rs java_package_name(foo)

typedef struct char_struct {
    short f1;
    // expect 6 bytes of padding here
    long f2;
} char_struct;

char_struct g_char_struct;

typedef struct five_struct {
    char f1;
    // expect 7 bytes of padding here
    long f2;
    short f3;
    // expect 6 bytes of padding here
    long f4;
    half f5;
    // expect 6 bytes of padding here
} five_struct;

five_struct g_five_struct;

bool failed = false;

void check_char_struct(short arg1, long arg2) {
    failed |= ((g_char_struct.f1 != arg1) || (g_char_struct.f2 != arg2));
}

void check_five_struct(char arg1, long arg2, short arg3, long arg4, half arg5) {
    failed |= ((g_five_struct.f1 != arg1) ||
               (g_five_struct.f2 != arg2) ||
               (g_five_struct.f3 != arg3) ||
               (g_five_struct.f4 != arg4) ||
               (g_five_struct.f5 != arg5));
}
