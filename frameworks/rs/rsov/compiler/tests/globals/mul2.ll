; RUN: rs2spirv_lit_driver.sh %s | FileCheck %s
target datalayout = "e-p:32:32-i64:64-v128:64:128-n32-S64"
target triple = "armv7-none-linux-gnueabi"

; CHECK: OpString ".rsov.ExportedVars:0;24;8;28;"

; CHECK-DAG: OpDecorate [[GPUBlockVar:%[a-zA-Z_0-9]+]] Binding 0
; CHECK-DAG: OpDecorate [[STRUCT:%struct__.*]] BufferBlock
; CHECK-DAG: OpMemberDecorate [[STRUCT]] 0 Offset 0
; CHECK-DAG: OpMemberDecorate [[STRUCT]] 1 Offset 8
; CHECK-DAG: OpMemberDecorate [[STRUCT]] 2 Offset 24
; CHECK-DAG: OpMemberDecorate [[STRUCT]] 3 Offset 28

; CHECK: [[STRUCT]] = OpTypeStruct %float %v4float %uchar %uint{{.*}}
; CHECK: [[STRUCT_PTR_TY:%[a-zA-Z_0-9]+]] = OpTypePointer Uniform [[STRUCT]]

; CHECK: [[GPUBlockVar]] = OpVariable [[STRUCT_PTR_TY]] Uniform

@c1 = common global float 0.000000e+00, align 4
@c2 = common global <4 x float> zeroinitializer, align 16
@d = common global i8 0, align 1
@i = common global i32 0, align 4

; Function Attrs: norecurse nounwind readonly
define <4 x float> @k1(<4 x float> %in) #0 {
entry:
  %0 = load float, float* @c1, align 4, !tbaa !14
  %splat.splatinsert = insertelement <4 x float> undef, float %0, i32 0
  %splat.splat = shufflevector <4 x float> %splat.splatinsert, <4 x float> undef, <4 x i32> zeroinitializer
  %mul = fmul <4 x float> %splat.splat, %in
  %1 = load <4 x float>, <4 x float>* @c2, align 16, !tbaa !18
  %mul1 = fmul <4 x float> %1, %mul
  ret <4 x float> %mul1
}

attributes #0 = { norecurse nounwind readonly "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="0" "stackrealign" "target-features"="+long64" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}
!\23pragma = !{!3, !4, !5}
!\23rs_export_var = !{!6, !7, !8, !9}
!\23rs_object_slots = !{}
!\23rs_export_foreach_name = !{!10, !11}
!\23rs_export_foreach = !{!12, !13}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"min_enum_size", i32 4}
!2 = !{!"Android clang version 3.8.256229  (based on LLVM 3.8.256229)"}
!3 = !{!"version", !"1"}
!4 = !{!"java_package_name", !"rs2spirv"}
!5 = !{!"rs_fp_relaxed", !""}
!6 = !{!"c1", !"1"}
!7 = !{!"d", !"7"}
!8 = !{!"c2", !"float4"}
!9 = !{!"i", !"5"}
!10 = !{!"root"}
!11 = !{!"k1"}
!12 = !{!"0"}
!13 = !{!"35"}
!14 = !{!15, !15, i64 0}
!15 = !{!"float", !16, i64 0}
!16 = !{!"omnipotent char", !17, i64 0}
!17 = !{!"Simple C/C++ TBAA"}
!18 = !{!16, !16, i64 0}
