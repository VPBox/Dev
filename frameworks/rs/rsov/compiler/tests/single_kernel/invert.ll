; RUN: rs2spirv_lit_driver.sh %s | FileCheck %s

target datalayout = "e-m:e-i64:64-i128:128-n32:64-S128"
target triple = "aarch64-none-linux-gnueabi"

; Function Attrs: norecurse nounwind readnone
; CHECK:OpDecorate %[[BUF_S:.*]] BufferBlock
; CHECK:OpDecorate %[[BUF_VAR:.*]] DescriptorSet {{[0-9]}}
; CHECK:OpDecorate %[[BUF_VAR]] Binding {{[0-9]}}
; CHECK:%[[BUF_PTR_TY:.*]] = OpTypePointer Uniform %[[BUF_S]]
; CHECK:%[[BUF_VAR]] = OpVariable %[[BUF_PTR_TY]] Uniform
; CHECK:%[[ADDR:.*]] = OpAccessChain {{.*}} %[[BUF_VAR]]
; CHECK:OpLoad {{.*}} %[[ADDR]]

define <4 x float> @invert(<4 x float> %in) #0 {
entry:
  %0 = extractelement <4 x float> %in, i64 0
  %sub = fsub float 1.000000e+00, %0
  %1 = insertelement <4 x float> undef, float %sub, i64 0
  %2 = extractelement <4 x float> %in, i64 1
  %sub1 = fsub float 1.000000e+00, %2
  %3 = insertelement <4 x float> %1, float %sub1, i64 1
  %4 = extractelement <4 x float> %in, i64 2
  %sub2 = fsub float 1.000000e+00, %4
  %5 = insertelement <4 x float> %3, float %sub2, i64 2
  %6 = extractelement <4 x float> %in, i64 3
  %sub3 = fsub float 1.000000e+00, %6
  %7 = insertelement <4 x float> %5, float %sub3, i64 3
  ret <4 x float> %7
}

attributes #0 = { norecurse nounwind readnone "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="0" "stackrealign" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}
!\23pragma = !{!1, !2, !3}
!\23rs_export_foreach_name = !{!4, !5}
!\23rs_export_foreach = !{!6, !7}

!0 = !{!"Android clang version 3.8.256229  (based on LLVM 3.8.256229)"}
!1 = !{!"version", !"1"}
!2 = !{!"java_package_name", !"rs2spirv"}
!3 = !{!"rs_fp_relaxed", !""}
!4 = !{!"root"}
!5 = !{!"invert"}
!6 = !{!"0"}
!7 = !{!"35"}
