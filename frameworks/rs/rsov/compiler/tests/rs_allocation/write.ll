; XFAIL: *
; RUN: rs2spirv_lit_driver.sh %s | FileCheck %s

target datalayout = "e-p:32:32-i64:64-v128:64:128-n32-S64"
target triple = "armv7-none-linux-gnueabi"

; CHECK: %struct___GPUBuffer = OpTypeStruct {{.*}}
; CHECK: [[PTR_TYPE:%[a-zA-Z_0-9]+]] = OpTypePointer Uniform %struct___GPUBuffer

; CHECK: %__GPUBlock = OpVariable [[PTR_TYPE]] Uniform

%struct.rs_allocation = type { i32* }

@alloc = common global %struct.rs_allocation zeroinitializer, align 4
@c1 = common global i8 0, align 1

; Function Attrs: nounwind
define <4 x i8> @k1(<4 x i8> %in, i32 %x, i32 %y) #0 {
entry:
  %0 = load i8, i8* @c1, align 1, !tbaa !13
  %splat.splatinsert = insertelement <4 x i8> undef, i8 %0, i32 0
  %splat.splat = shufflevector <4 x i8> %splat.splatinsert, <4 x i8> undef, <4 x i32> zeroinitializer
  %add = add <4 x i8> %splat.splat, %in
  %.unpack = load i32, i32* bitcast (%struct.rs_allocation* @alloc to i32*), align 4
  %1 = insertvalue [1 x i32] undef, i32 %.unpack, 0
  tail call void @_Z21rsSetElementAt_uchar413rs_allocationDv4_hjj([1 x i32] %1, <4 x i8> %add, i32 %x, i32 %y) #2
  ret <4 x i8> %in
}

declare void @_Z21rsSetElementAt_uchar413rs_allocationDv4_hjj([1 x i32], <4 x i8>, i32, i32) #1

; Function Attrs: nounwind
define void @.rs.dtor() #0 {
entry:
  tail call void @_Z13rsClearObjectP13rs_allocation(%struct.rs_allocation* nonnull @alloc) #2
  ret void
}

declare void @_Z13rsClearObjectP13rs_allocation(%struct.rs_allocation*) #1

attributes #0 = { nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="0" "stackrealign" "target-features"="+long64" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="0" "stackrealign" "target-features"="+long64" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}
!\23pragma = !{!3, !4, !5}
!\23rs_export_var = !{!6, !7}
!\23rs_object_slots = !{!8}
!\23rs_export_foreach_name = !{!9, !10}
!\23rs_export_foreach = !{!11, !12}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"min_enum_size", i32 4}
!2 = !{!"Android clang version 3.8.256229  (based on LLVM 3.8.256229)"}
!3 = !{!"version", !"1"}
!4 = !{!"java_package_name", !"rs2spirv"}
!5 = !{!"rs_fp_relaxed", !""}
!6 = !{!"c1", !"7"}
!7 = !{!"alloc", !"20"}
!8 = !{!"1"}
!9 = !{!"root"}
!10 = !{!"k1"}
!11 = !{!"0"}
!12 = !{!"59"}
!13 = !{!14, !14, i64 0}
!14 = !{!"omnipotent char", !15, i64 0}
!15 = !{!"Simple C/C++ TBAA"}
