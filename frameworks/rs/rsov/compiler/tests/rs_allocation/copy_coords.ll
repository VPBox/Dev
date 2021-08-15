; RUN: rs2spirv_lit_driver.sh %s | FileCheck %s

target datalayout = "e-p:32:32-i64:64-v128:64:128-n32-S64"
target triple = "armv7-none-linux-gnueabi"

; CHECK: [[InvocationID:%.*]] = OpLoad %v3uint %gl_GlobalInvocationID
; CHECK: {{%.*}} = OpCompositeExtract %uint [[InvocationID]] 0
; CHECK: {{%.*}} = OpCompositeExtract %uint [[InvocationID]] 1

; Function Attrs: norecurse nounwind readnone
define <4 x i8> @copy_coords(<4 x i8> %in, i32 %x, i32 %y) #0 {
entry:
  %conv = trunc i32 %x to i8
  %0 = insertelement <4 x i8> %in, i8 %conv, i32 0
  %conv1 = trunc i32 %y to i8
  %1 = insertelement <4 x i8> %0, i8 %conv1, i32 1
  ret <4 x i8> %1
}

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
!7 = !{!"copy_coords"}
!8 = !{!"0"}
!9 = !{!"59"}
