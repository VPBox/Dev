; RUN: rs2spirv_lit_driver.sh %s | FileCheck %s
target datalayout = "e-p:32:32-i64:64-v128:64:128-n32-S64"
target triple = "armv7-none-linux-gnueabi"

; CHECK: OpString ".rsov.ExportedVars:0;"

; CHECK-DAG: OpDecorate [[GPUBlockVar:%[a-zA-Z_0-9]+]] Binding 0
; CHECK-DAG: OpDecorate [[STRUCT:%struct__.*]] BufferBlock
; CHECK-DAG: OpMemberDecorate [[STRUCT]] 0 Offset 0

; CHECK: [[STRUCT]] = OpTypeStruct %float
; CHECK: [[STRUCT_PTR_TY:%[a-zA-Z_0-9]+]] = OpTypePointer Uniform [[STRUCT]]

; CHECK: [[GPUBlockVar]] = OpVariable [[STRUCT_PTR_TY]] Uniform

@c1 = common global float 0.000000e+00, align 4

; Function Attrs: norecurse nounwind readonly
define <4 x float> @k1(<4 x float> %in) #0 {
entry:
  %0 = load float, float* @c1, align 4, !tbaa !11
  %splat.splatinsert = insertelement <4 x float> undef, float %0, i32 0
  %splat.splat = shufflevector <4 x float> %splat.splatinsert, <4 x float> undef, <4 x i32> zeroinitializer
  %mul = fmul <4 x float> %splat.splat, %in
  ret <4 x float> %mul
}

attributes #0 = { norecurse nounwind readonly "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="0" "stackrealign" "target-features"="+long64" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}
!\23pragma = !{!3, !4, !5}
!\23rs_export_var = !{!6}
!\23rs_object_slots = !{}
!\23rs_export_foreach_name = !{!7, !8}
!\23rs_export_foreach = !{!9, !10}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"min_enum_size", i32 4}
!2 = !{!"Android clang version 3.8.256229  (based on LLVM 3.8.256229)"}
!3 = !{!"version", !"1"}
!4 = !{!"java_package_name", !"rs2spirv"}
!5 = !{!"rs_fp_relaxed", !""}
!6 = !{!"c1", !"1"}
!7 = !{!"root"}
!8 = !{!"k1"}
!9 = !{!"0"}
!10 = !{!"35"}
!11 = !{!12, !12, i64 0}
!12 = !{!"float", !13, i64 0}
!13 = !{!"omnipotent char", !14, i64 0}
!14 = !{!"Simple C/C++ TBAA"}
