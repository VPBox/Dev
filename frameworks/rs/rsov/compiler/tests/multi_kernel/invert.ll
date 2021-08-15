; RUN: rs2spirv_lit_driver.sh %s | FileCheck %s

target datalayout = "e-p:32:32-i64:64-v128:64:128-n32-S64"
target triple = "armv7-none-linux-gnueabi"

; CHECK: OpEntryPoint GLCompute [[FuncId1:%[0-9]+]] "entry_invert" %gl_GlobalInvocationID
; CHECK-NEXT: OpEntryPoint GLCompute [[FuncId2:%[0-9]+]] "entry_scalebytwo" %gl_GlobalInvocationID

; Multi-kernel testcase
;float4 RS_KERNEL invert(float4 a) {
;    return 1.0f - a;
;}

; CHECK: [[FuncId1]] = OpFunction %void None
; Function Attrs: norecurse nounwind readnone
define <4 x float> @invert(<4 x float> %a) local_unnamed_addr #0 {
entry:
  %sub = fsub <4 x float> <float 1.000000e+00, float 1.000000e+00, float 1.000000e+00, float 1.000000e+00>, %a
  ret <4 x float> %sub
}


; CHECK: [[FuncId2]] = OpFunction %void None
;float RS_KERNEL scalebytwo(float a) {
;    return a*2;
;}

; Function Attrs: norecurse nounwind readnone
define float @scalebytwo(float %a) local_unnamed_addr #0 {
entry:
  %mul = fmul float %a, 2.000000e+00
  ret float %mul
}

attributes #0 = { norecurse nounwind readnone "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="0" "stackrealign" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}
!\23pragma = !{!3, !4}
!\23rs_export_foreach_name = !{!5, !6, !7}
!\23rs_export_foreach = !{!8, !9, !9}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"min_enum_size", i32 4}
!2 = !{!"Android clang version 3.8.275480  (based on LLVM 3.8.275480)"}
!3 = !{!"version", !"1"}
!4 = !{!"java_package_name", !"com.android.rs.rsov.test"}
!5 = !{!"root"}
!6 = !{!"invert"}
!7 = !{!"scalebytwo"}
!8 = !{!"0"}
!9 = !{!"35"}
