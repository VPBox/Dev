// RUN: %Slang %s

// RUN: %rs-filecheck-wrapper %s --check-prefix=CHECK-LL
//     Check that the data type small_struct_2 is properly padded:
// CHECK-LL: %struct.small_struct_2{{(\.[0-9]+)?}} = type { i64, i32, [4 x i8] }
//     Check that the helper function for unpacking an invokable's arguments
//     accesses a properly (un)padded struct:
// CHECK-LL: define void @.helper_checkStruct({ i64, i32, [4 x i8] }* nocapture)
// CHECK-LL: [[FIELD_L_ADDR:%[0-9]+]] = getelementptr inbounds { i64, i32, [4 x i8] }, { i64, i32, [4 x i8] }* %0, i{{[0-9]+}} 0, i32 0
// CHECK-LL: [[FIELD_L_VAL:%[0-9]+]] = load i64, i64* [[FIELD_L_ADDR]]
// CHECK-LL: [[FIELD_I_ADDR:%[0-9]+]] = getelementptr inbounds { i64, i32, [4 x i8] }, { i64, i32, [4 x i8] }* %0, i{{[0-9]+}} 0, i32 1
// CHECK-LL: [[FIELD_I_VAL:%[0-9]+]] = load i32, i32* [[FIELD_I_ADDR]]
// CHECK-LL: call void @checkStruct(i64 [[FIELD_L_VAL]], i32 [[FIELD_I_VAL]])

// RUN: %scriptc-filecheck-wrapper --lang=Java --type=small_struct_2 --check-prefix=CHECK-JAVA-STRUCT %s
// CHECK-JAVA-STRUCT:      public static Element createElement(RenderScript rs) {
// CHECK-JAVA-STRUCT-NEXT:     Element.Builder eb = new Element.Builder(rs);
// CHECK-JAVA-STRUCT-NEXT:     eb.add(Element.I64(rs), "l");
// CHECK-JAVA-STRUCT-NEXT:     eb.add(Element.I32(rs), "i");
// CHECK-JAVA-STRUCT-NEXT:     eb.add(Element.U32(rs), "#rs_padding_1");
// CHECK-JAVA-STRUCT-NEXT:     return eb.create();
// CHECK-JAVA-STRUCT-NEXT: }

// RUN: %scriptc-filecheck-wrapper --lang=Java --check-prefix=CHECK-JAVA-INVOKE %s
// CHECK-JAVA-INVOKE:      public void invoke_checkStruct(long argL, int argI) {
// CHECK-JAVA-INVOKE-NEXT:     FieldPacker checkStruct_fp = new FieldPacker(16);
// CHECK-JAVA-INVOKE-NEXT:     checkStruct_fp.addI64(argL);
// CHECK-JAVA-INVOKE-NEXT:     checkStruct_fp.addI32(argI);
// CHECK-JAVA-INVOKE-NEXT:     checkStruct_fp.skip(4);
// CHECK-JAVA-INVOKE-NEXT:     invoke(mExportFuncIdx_checkStruct, checkStruct_fp);
// CHECK-JAVA-INVOKE-NEXT: }

// Same as small_struct.rs except for order of fields (hence location of padding) in struct small_struct[_2].

#pragma version(1)
#pragma rs java_package_name(foo)

typedef struct small_struct_2 {
    long l;
    int i;
    // expect 4 bytes of padding here
} small_struct_2;

small_struct_2 g_small_struct_2;

bool failed = false;

void checkStruct(long argL, int argI) {
    failed |= ((g_small_struct_2.l != argL) || (g_small_struct_2.i != argI));
}
