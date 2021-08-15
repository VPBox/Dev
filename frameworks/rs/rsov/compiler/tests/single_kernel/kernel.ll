; RUN: rs2spirv_lit_driver.sh %s | FileCheck %s

target datalayout = "e-m:e-i64:64-i128:128-n32:64-S128"
target triple = "aarch64-none-linux-gnueabi"

; CHECK: OpEntryPoint GLCompute [[WrapperId:%[a-zA-Z_0-9]*]] "entry_foo"
; CHECK: [[KernelId:%[a-zA-Z_0-9]+]] = OpFunction {{.*}}

; Function Attrs: norecurse nounwind readnone
define void @foo() #0 {
  ret void
}
; CHECK: [[WrapperId]] = OpFunction {{.*}}
; CHECK-NEXT: OpLabel
; CHECK-NEXT: %{{[0-9]+}} = OpFunctionCall %{{.*}} [[KernelId]]
; CHECK-NEXT: OpReturn

attributes #0 = { norecurse nounwind readnone "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="0" "stackrealign" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}
!\23pragma = !{!1, !2, !3}
!\23rs_export_foreach_name = !{!4, !5}
!\23rs_export_foreach = !{!6, !7}

!0 = !{!"Android clang version 3.8.256229  (based on LLVM 3.8.256229)"}
!1 = !{!"version", !"1"}
!2 = !{!"java_package_name", !"rs2srpiv"}
!3 = !{!"rs_fp_relaxed", !""}
!4 = !{!"root"}
!5 = !{!"foo"}
!6 = !{!"0"}
!7 = !{!"32"}
