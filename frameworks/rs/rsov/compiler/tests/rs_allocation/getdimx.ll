; RUN: rs2spirv_lit_driver.sh %s | FileCheck %s

; rs_allocation g;
; extern uint32_t __rsov_rsAllocationGetDimX(uint32_t ga);
; int32_t RS_KERNEL getDim(int32_t dummy) {
;    // lowered accessor by an earlier LLVM pass
;    return __rsov_rsAllocationGetDimX(0);
;}
target datalayout = "e-p:32:32-i64:64-v128:64:128-n32-S64"
target triple = "armv7-none-linux-gnueabi"

%struct.rs_allocation = type { i32* }
; CHECK-NOT OpCapability Kernel
; CHECK-NOT OpDecorate %{{[a-zA-Z_0-9]*}} Alignment {{[0-9]*}}
; CHECK: OpMemberDecorate [[MetadataS:%[a-zA-Z_0-9]*]] 0 Offset 4
; CHECK: OpMemberDecorate [[MetadataS]] 1 Offset 8
; CHECK: OpMemberDecorate [[MetadataS]] 2 Offset 12
; CHECK: OpMemberDecorate [[MetadataS]] 3 Offset 16
; CHECK: OpDecorate [[RuntimeArrS:%[a-zA-Z_0-9]*]] ArrayStride {{[0-9]*}}
; CHECK: OpDecorate [[MetadataSSBO:%[a-zA-Z_0-9]*]] BufferBlock
; CHECK: OpDecorate [[Metadata:%[a-zA-Z_0-9]*]] DescriptorSet 0
; CHECK: OpDecorate [[Metadata]] Binding 1


; CHECK: [[MetadataSSBO]] = OpTypeStruct [[RuntimeArrS]]
; CHECK: [[MetadataPtr:%[a-zA-Z_0-9]*]] = OpTypePointer Uniform [[MetadataSSBO]]
; CHECK: [[Metadata]] = OpVariable [[MetadataPtr]]

@g = common global %struct.rs_allocation zeroinitializer, align 4


; CHECK-NOT OpFunctionCall %uint %__rsov_rsAllocationGetDimX %{{[a-zA-Z_0-9]*}}
; CHECK: [[DimX:%[a-zA-Z_0-9]*]] = OpAccessChain %_ptr_Uniform_uint [[Metadata]]
; CHECK: [[Res:%[a-zA-Z_0-9]*]] = OpLoad %uint [[DimX]]
; CHECK: OpReturnValue [[Res]]

; Function Attrs: nounwind
define i32 @getDim(i32 %dummy) local_unnamed_addr #0 {
entry:
  %call = tail call i32 @__rsov_rsAllocationGetDimX(i32 0) #2
  ret i32 %call
}

declare i32 @__rsov_rsAllocationGetDimX(i32) local_unnamed_addr #1

; Function Attrs: nounwind
define void @.rs.dtor() local_unnamed_addr #0 {
entry:
  tail call void @_Z13rsClearObjectP13rs_allocation(%struct.rs_allocation* nonnull @g) #2
  ret void
}

declare void @_Z13rsClearObjectP13rs_allocation(%struct.rs_allocation*) local_unnamed_addr #1

attributes #0 = { nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="0" "stackrealign" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="0" "stackrealign" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}
!\23pragma = !{!3, !4}
!\23rs_export_var = !{!5}
!\23rs_object_slots = !{!6}
!\23rs_export_foreach_name = !{!7, !8}
!\23rs_export_foreach = !{!6, !9}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"min_enum_size", i32 4}
!2 = !{!"Android clang version 3.8.275480  (based on LLVM 3.8.275480)"}
!3 = !{!"version", !"1"}
!4 = !{!"java_package_name", !"com.android.rs.rsov.test"}
!5 = !{!"g", !"20"}
!6 = !{!"0"}
!7 = !{!"root"}
!8 = !{!"getDim"}
!9 = !{!"35"}
