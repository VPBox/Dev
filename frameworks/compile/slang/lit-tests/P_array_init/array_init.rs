// RUN: %Slang %s
// RUN: %rs-filecheck-wrapper %s
// CHECK: @ic = local_unnamed_addr constant i32 99, align 4
// CHECK: @ica = local_unnamed_addr global [2 x i32] [i32 99, i32 1000], align 4
// CHECK: @fa = local_unnamed_addr global [4 x float] [float 1.000000e+00, float 0x4023FFF2E0000000, float 0.000000e+00, float 0.000000e+00], align 4
// CHECK: @da = local_unnamed_addr global [2 x double] [double 7.000000e+00, double 8.888880e+00], align 8
// CHECK: @ca = local_unnamed_addr global [4 x i8] c"a\07bc", align 1
// CHECK: @sa = local_unnamed_addr global [4 x i16] [i16 1, i16 1, i16 2, i16 3], align 2
// CHECK: @ia = local_unnamed_addr global [4 x i32] [i32 5, i32 8, i32 0, i32 0], align 4
// CHECK: @la = local_unnamed_addr global [2 x i64] [i64 13, i64 21], align 8
// CHECK: @lla = local_unnamed_addr global [4 x i64] [i64 34, i64 0, i64 0, i64 0], align 8
// CHECK: @ba = local_unnamed_addr global [3 x i8] c"\01\00\00", align 1


#pragma version(1)
#pragma rs java_package_name(array_init)

const int ic = 99;

int ica[2] = {ic, 1000};

float fa[4] = {1.0, 9.9999f};
double da[2] = {7.0, 8.88888};
char ca[4] = {'a', 7, 'b', 'c'};
short sa[4] = {1, 1, 2, 3};
int ia[4] = {5, 8};
long la[2] = {13, 21};
long long lla[4] = {34};
bool ba[3] = {true, false};

