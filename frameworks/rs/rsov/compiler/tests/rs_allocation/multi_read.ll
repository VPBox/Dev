; XFAIL: *
; RUN: rs2spirv_lit_driver.sh %s | FileCheck %s

target datalayout = "e-p:32:32-i64:64-v128:64:128-n32-S64"
target triple = "armv7-none-linux-gnueabi"

; CHECK: %struct___GPUBuffer = OpTypeStruct %uchar
; CHECK: [[PTR_TYPE:%[a-zA-Z_0-9]+]] = OpTypePointer Uniform %struct___GPUBuffer

; CHECK: %__GPUBlock = OpVariable [[PTR_TYPE]] Uniform

%struct.rs_allocation = type { i32* }

@a1 = common global %struct.rs_allocation zeroinitializer, align 4
@a2 = common global %struct.rs_allocation zeroinitializer, align 4
@c1 = common global i8 0, align 1

; Function Attrs: nounwind
define <4 x i8> @k1(<4 x i8> %in) #0 {
entry:
  %0 = extractelement <4 x i8> %in, i32 0
  %conv = zext i8 %0 to i32
  %1 = extractelement <4 x i8> %in, i32 1
  %conv1 = zext i8 %1 to i32
  %.unpack = load i32, i32* bitcast (%struct.rs_allocation* @a1 to i32*), align 4
  %2 = insertvalue [1 x i32] undef, i32 %.unpack, 0
  %call = tail call <4 x i8> @_Z21rsGetElementAt_uchar413rs_allocationjj([1 x i32] %2, i32 %conv, i32 %conv1) #2
  %3 = extractelement <4 x i8> %in, i32 2
  %conv2 = zext i8 %3 to i32
  %4 = extractelement <4 x i8> %in, i32 3
  %conv3 = zext i8 %4 to i32
  %.unpack16 = load i32, i32* bitcast (%struct.rs_allocation* @a2 to i32*), align 4
  %5 = insertvalue [1 x i32] undef, i32 %.unpack16, 0
  %call4 = tail call <4 x i8> @_Z21rsGetElementAt_uchar413rs_allocationjj([1 x i32] %5, i32 %conv2, i32 %conv3) #2
  %add = add <4 x i8> %call4, %call
  %6 = load i8, i8* @c1, align 1, !tbaa !15
  %conv5 = zext i8 %6 to i32
  %7 = extractelement <4 x i8> %add, i32 2
  %conv6 = zext i8 %7 to i32
  %add7 = add nuw nsw i32 %conv6, %conv5
  %conv8 = trunc i32 %add7 to i8
  %8 = insertelement <4 x i8> %add, i8 %conv8, i32 2
  ret <4 x i8> %8
}

declare <4 x i8> @_Z21rsGetElementAt_uchar413rs_allocationjj([1 x i32], i32, i32) #1

; Function Attrs: nounwind
define void @.rs.dtor() #0 {
entry:
  tail call void @_Z13rsClearObjectP13rs_allocation(%struct.rs_allocation* nonnull @a1) #2
  tail call void @_Z13rsClearObjectP13rs_allocation(%struct.rs_allocation* nonnull @a2) #2
  ret void
}

declare void @_Z13rsClearObjectP13rs_allocation(%struct.rs_allocation*) #1

attributes #0 = { nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="0" "stackrealign" "target-features"="+long64" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="0" "stackrealign" "target-features"="+long64" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}
!\23pragma = !{!3, !4, !5}
!\23rs_export_var = !{!6, !7, !8}
!\23rs_object_slots = !{!9, !10}
!\23rs_export_foreach_name = !{!11, !12}
!\23rs_export_foreach = !{!13, !14}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"min_enum_size", i32 4}
!2 = !{!"Android clang version 3.8.256229  (based on LLVM 3.8.256229)"}
!3 = !{!"version", !"1"}
!4 = !{!"java_package_name", !"rs2spirv"}
!5 = !{!"rs_fp_relaxed", !""}
!6 = !{!"c1", !"7"}
!7 = !{!"a1", !"20"}
!8 = !{!"a2", !"20"}
!9 = !{!"1"}
!10 = !{!"2"}
!11 = !{!"root"}
!12 = !{!"k1"}
!13 = !{!"0"}
!14 = !{!"35"}
!15 = !{!16, !16, i64 0}
!16 = !{!"omnipotent char", !17, i64 0}
!17 = !{!"Simple C/C++ TBAA"}
