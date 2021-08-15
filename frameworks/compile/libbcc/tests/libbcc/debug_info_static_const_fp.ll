; Per https://llvm.org/bugs/show_bug.cgi?id=26619, check that for DIGlobalVariable of floating-point type,
; we emit its constant value (if any) in debug info.

; Debug metadata doesn't survive passage through llvm-rs-as.  Therefore, we represent
;   the test case as a separate bitcode file, rather than as the ASCII IR in this file.
; There doesn't seem to be any way to compute the basename of %s, hence the unfortunate
;   explicit uses of "debug_info_static_const_fp" below.

; RUN: bcc -o debug_info_static_const_fp -output_path %T -bclib libclcore.bc -mtriple armv7-none-linux-gnueabi %S/debug_info_static_const_fp.bc
; RUN: llvm-dwarfdump -debug-dump=info %T/debug_info_static_const_fp.o | FileCheck %s

; 32-bit bitcode compiled from the following (-O0 -g):
;
;   #pragma version(1)
;   #pragma rs java_package_name(debug_info_static_const_fp)
;
;   static const __fp16 hVal = 29/13.0f;            //    2.2307692307692307692     (2.23046875)
;
;   static const float fVal = -147/17.0f;           //   -8.6470588235294117647     (-8.64705849)
;
;   static const double dVal = 19637/7.0;           // 2805.2857142857142857        (2805.2857142857142)
;
;   int iVal;
;
;   void compute() {
;     iVal = hVal + fVal + dVal;
;   }

; CHECK: DW_TAG_variable
; CHECK-NOT: {{DW_TAG|NULL}}
; CHECK: DW_AT_name {{.*}} "hVal"
; CHECK-NOT: {{DW_TAG|NULL}}
; CHECK: DW_AT_const_value {{.*}} (16502)

; CHECK: DW_TAG_variable
; CHECK-NOT: {{DW_TAG|NULL}}
; CHECK: DW_AT_name {{.*}} "fVal"
; CHECK-NOT: {{DW_TAG|NULL}}
; CHECK: DW_AT_const_value {{.*}} (3238681178)

; CHECK: DW_TAG_variable
; CHECK-NOT: {{DW_TAG|NULL}}
; CHECK: DW_AT_name {{.*}} "dVal"
; CHECK-NOT: {{DW_TAG|NULL}}
; CHECK: DW_AT_const_value {{.*}} (4658387303597904457)
