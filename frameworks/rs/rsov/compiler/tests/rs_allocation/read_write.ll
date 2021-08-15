; XFAIL: *
; RUN: rs2spirv_lit_driver.sh %s | FileCheck %s

target datalayout = "e-p:32:32-i64:64-v128:64:128-n32-S64"
target triple = "armv7-none-linux-gnueabi"

; CHECK: %struct___GPUBuffer = OpTypeStruct %uchar
; CHECK: [[PTR_TYPE:%[a-zA-Z_0-9]+]] = OpTypePointer Uniform %struct___GPUBuffer

; CHECK: %__GPUBlock = OpVariable [[PTR_TYPE]] Uniform

%struct.rs_allocation = type { i32* }

@r = common global %struct.rs_allocation zeroinitializer, align 4
@w = common global %struct.rs_allocation zeroinitializer, align 4
@c1 = common global i8 0, align 1

; Function Attrs: nounwind
define <4 x i8> @k1(<4 x i8> %in, i32 %x, i32 %y) #0 {
entry:
  %.unpack = load i32, i32* bitcast (%struct.rs_allocation* @r to i32*), align 4
  %0 = insertvalue [1 x i32] undef, i32 %.unpack, 0
  %call = tail call <4 x i8> @_Z21rsGetElementAt_uchar413rs_allocationjj([1 x i32] %0, i32 %x, i32 %y) #2
  %1 = load i8, i8* @c1, align 1, !tbaa !15
  %splat.splatinsert = insertelement <4 x i8> undef, i8 %1, i32 0
  %splat.splat = shufflevector <4 x i8> %splat.splatinsert, <4 x i8> undef, <4 x i32> zeroinitializer
  %add = add <4 x i8> %call, %in
  %add1 = add <4 x i8> %add, %splat.splat
  %.unpack6 = load i32, i32* bitcast (%struct.rs_allocation* @w to i32*), align 4
  %2 = insertvalue [1 x i32] undef, i32 %.unpack6, 0
  tail call void @_Z21rsSetElementAt_uchar413rs_allocationDv4_hjj([1 x i32] %2, <4 x i8> %add1, i32 %x, i32 %y) #2
  ret <4 x i8> %in
}

declare <4 x i8> @_Z21rsGetElementAt_uchar413rs_allocationjj([1 x i32], i32, i32) #1

declare void @_Z21rsSetElementAt_uchar413rs_allocationDv4_hjj([1 x i32], <4 x i8>, i32, i32) #1

; Function Attrs: nounwind
define void @.rs.dtor() #0 {
entry:
  tail call void @_Z13rsClearObjectP13rs_allocation(%struct.rs_allocation* nonnull @r) #2
  tail call void @_Z13rsClearObjectP13rs_allocation(%struct.rs_allocation* nonnull @w) #2
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
!7 = !{!"r", !"20"}
!8 = !{!"w", !"20"}
!9 = !{!"1"}
!10 = !{!"2"}
!11 = !{!"root"}
!12 = !{!"k1"}
!13 = !{!"0"}
!14 = !{!"59"}
!15 = !{!16, !16, i64 0}
!16 = !{!"omnipotent char", !17, i64 0}
!17 = !{!"Simple C/C++ TBAA"}
