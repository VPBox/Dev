; RUN: rs2spirv_lit_driver.sh %s | FileCheck %s

target datalayout = "e-p:32:32-i64:64-v128:64:128-n32-S64"
target triple = "armv7-none-linux-gnueabi"

; CHECK: OpEntryPoint GLCompute [[WrapperId:%[a-zA-Z_0-9]*]] "entry_copy"
; CHECK: [[KernelId:%[a-zA-Z_0-9]+]] = OpFunction {{.*}}

; Function Attrs: norecurse nounwind readnone
define <4 x i8> @copy(<4 x i8> %v_in) #0 {
entry:
  ret <4 x i8> %v_in
}

; CHECK: [[WrapperId]] = OpFunction {{.*}}
; CHECK-NEXT: OpLabel
; CHECK: %{{[0-9]+}} = OpFunctionCall %{{.*}} [[KernelId]]
; CHECK: OpReturn

attributes #0 = { norecurse nounwind readnone "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="0" "stackrealign" "target-features"="+long64" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}
!\23pragma = !{!3, !4, !5}
!\23rs_export_foreach_name = !{!6, !7}
!\23rs_export_foreach = !{!8, !9}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"min_enum_size", i32 4}
!2 = !{!"Android clang version 3.8.256229  (based on LLVM 3.8.256229)"}
!3 = !{!"version", !"1"}
!4 = !{!"java_package_name", !"rs2spirv"}
!5 = !{!"rs_fp_relaxed", !""}
!6 = !{!"root"}
!7 = !{!"copy"}
!8 = !{!"0"}
!9 = !{!"35"}
