; RUN: rs2spirv_lit_driver.sh %s | FileCheck %s
; Source:
; rs_allocation g;
; int32_t RS_KERNEL getDim(int32_t dummy) {
;    return rsAllocationGetDimX(g);
; }
target datalayout = "e-m:e-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128"
target triple = "aarch64-none-linux-gnueabi"

; CHECK: OpString "{\"__RSoV_GA\": {\"g\":0}}"
; CHECK: OpMemberDecorate [[MetadataS:%[a-zA-Z_0-9]*]] 0 Offset 4
; CHECK: OpMemberDecorate [[MetadataS]] 1 Offset 8
; CHECK: OpMemberDecorate [[MetadataS]] 2 Offset 12
; CHECK: OpMemberDecorate [[MetadataS]] 3 Offset 16
; CHECK: OpDecorate [[RuntimeArrS:%[a-zA-Z_0-9]*]] ArrayStride {{[0-9]*}}
; CHECK: OpDecorate [[MetadataSSBO:%[a-zA-Z_0-9]*]] BufferBlock
; CHECK: OpDecorate [[Metadata:%[a-zA-Z_0-9]*]] DescriptorSet 0
; CHECK: OpDecorate [[Metadata]] Binding 1

%struct.rs_allocation.1 = type { i64*, i64*, i64*, i64* }

@g = common global %struct.rs_allocation.1 zeroinitializer, align 8
; CHECK-NOT: %g = OpVariable %{{.*}} Uniform
; CHECK-NOT: OpCopyMemorySized
; CHECK-NOT: OpFunctionCall %uint %__rsov_rsAllocationGetDimX
; CHECK: [[DimX:%[a-zA-Z_0-9]*]] = OpAccessChain %_ptr_Uniform_uint [[Metadata]]
; CHECK: [[Res:%[a-zA-Z_0-9]*]] = OpLoad %uint [[DimX]]
; CHECK: OpReturnValue [[Res]]

; Function Attrs: nounwind
define i32 @getDim(i32 %dummy) unnamed_addr #0 {
entry:
  %tmp = alloca %struct.rs_allocation.1, align 8
  %0 = bitcast %struct.rs_allocation.1* %tmp to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast (%struct.rs_allocation.1* @g to i8*), i64 32, i32 8, i1 false), !tbaa.struct !8
  %call = call i32 @_Z19rsAllocationGetDimX13rs_allocation(%struct.rs_allocation.1* %tmp) #0
  ret i32 %call
}

declare i32 @_Z19rsAllocationGetDimX13rs_allocation(%struct.rs_allocation.1*) unnamed_addr

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i32, i1) #1

; Function Attrs: nounwind
define void @.rs.dtor() unnamed_addr #0 {
entry:
  tail call void @_Z13rsClearObjectP13rs_allocation(%struct.rs_allocation.1* @g) #0
  ret void
}

declare void @_Z13rsClearObjectP13rs_allocation(%struct.rs_allocation.1*) unnamed_addr

attributes #0 = { nounwind }
attributes #1 = { argmemonly nounwind }

!llvm.ident = !{!0}
!\23pragma = !{!1, !2}
!\23rs_export_var = !{!3}
!\23rs_object_slots = !{!4}
!\23rs_export_foreach_name = !{!5, !6}
!\23rs_export_foreach = !{!4, !7}

!0 = !{!"Android clang version 3.8.275480  (based on LLVM 3.8.275480)"}
!1 = !{!"version", !"1"}
!2 = !{!"java_package_name", !"com.android.rs.rsov.test"}
!3 = !{!"g", !"20"}
!4 = !{!"0"}
!5 = !{!"root"}
!6 = !{!"getDim"}
!7 = !{!"35"}
!8 = !{i64 0, i64 8, !9, i64 8, i64 8, !9, i64 16, i64 8, !9, i64 24, i64 8, !9}
!9 = !{!10, !10, i64 0}
!10 = !{!"any pointer", !11, i64 0}
!11 = !{!"omnipotent char", !12, i64 0}
!12 = !{!"Simple C/C++ TBAA"}
