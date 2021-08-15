; RUN: rs2spirv_lit_driver.sh %s | FileCheck %s

; CHECK: OpDecorate [[InVar1:%.*]] Binding 3
; CHECK: OpDecorate [[OutVar1:%.*]] Binding 2

; CHECK: OpDecorate [[InVar2:%.*]] Binding 3
; CHECK: OpDecorate [[OutVar2:%.*]] Binding 2

; CHECK: OpDecorate [[InVar3:%.*]] Binding 3
; CHECK: OpDecorate [[OutVar3:%.*]] Binding 2

; CHECK: OpDecorate [[InVar4:%.*]] Binding 3
; CHECK: OpDecorate [[OutVar4:%.*]] Binding 2

; CHECK: [[MemberTy1:%.*]] = OpTypeRuntimeArray %v4uint
; CHECK: [[Struct1:%.*]] = OpTypeStruct [[MemberTy1]]
; CHECK: [[PtrTy1:%.*]] = OpTypePointer Uniform [[Struct1]]
; CHECK: [[InVar1]] = OpVariable [[PtrTy1]]

; CHECK: [[OutMemberTy1:%.*]] = OpTypeRuntimeArray %v4uint
; CHECK: [[OutStruct1:%.*]] = OpTypeStruct [[OutMemberTy1]]
; CHECK: [[OutPtrTy1:%.*]] = OpTypePointer Uniform [[OutStruct1]]
; CHECK: [[OutVar1]] = OpVariable [[OutPtrTy1]]

; CHECK: [[MemberTy2:%.*]] = OpTypeRuntimeArray %v4uint
; CHECK: [[Struct2:%.*]] = OpTypeStruct [[MemberTy2]]
; CHECK: [[PtrTy2:%.*]] = OpTypePointer Uniform [[Struct2]]
; CHECK: [[InVar2]] = OpVariable [[PtrTy2]]

; CHECK: [[OutMemberTy2:%.*]] = OpTypeRuntimeArray %v4uint
; CHECK: [[OutStruct2:%.*]] = OpTypeStruct [[OutMemberTy2]]
; CHECK: [[OutPtrTy2:%.*]] = OpTypePointer Uniform [[OutStruct2]]
; CHECK: [[OutVar2]] = OpVariable [[OutPtrTy2]]

; CHECK: [[MemberTy3:%.*]] = OpTypeRuntimeArray %uint
; CHECK: [[Struct3:%.*]] = OpTypeStruct [[MemberTy3]]
; CHECK: [[PtrTy3:%.*]] = OpTypePointer Uniform [[Struct3]]
; CHECK: [[InVar3]] = OpVariable [[PtrTy3]]

; CHECK: [[OutMemberTy3:%.*]] = OpTypeRuntimeArray %uint
; CHECK: [[OutStruct3:%.*]] = OpTypeStruct [[OutMemberTy3]]
; CHECK: [[OutPtrTy3:%.*]] = OpTypePointer Uniform [[OutStruct3]]
; CHECK: [[OutVar3]] = OpVariable [[OutPtrTy3]]

; CHECK: [[MemberTy4:%.*]] = OpTypeRuntimeArray %uint
; CHECK: [[Struct4:%.*]] = OpTypeStruct [[MemberTy4]]
; CHECK: [[PtrTy4:%.*]] = OpTypePointer Uniform [[Struct4]]
; CHECK: [[InVar4]] = OpVariable [[PtrTy4]]

; CHECK: [[OutMemberTy4:%.*]] = OpTypeRuntimeArray %uint
; CHECK: [[OutStruct4:%.*]] = OpTypeStruct [[OutMemberTy4]]
; CHECK: [[OutPtrTy4:%.*]] = OpTypePointer Uniform [[OutStruct4]]
; CHECK: [[OutVar4]] = OpVariable [[OutPtrTy4]]

; // the RS the .rs from which this .ll is generated
; #pragma version(1)
; #pragma rs java_package_name(com.android.rs.test)

; int4 RS_KERNEL increment4(int4 in)
; {
;     return in + 1;
; }

; uint4 RS_KERNEL uincrement4(uint4 in)
; {
;     return in + 1;
; }

; int RS_KERNEL increment(int in)
; {
;     return in + 1;
; }

; unsigned RS_KERNEL uincrement(unsigned in)
; {
;     return in + 1;
; }


target datalayout = "e-p:32:32-i64:64-v128:64:128-n32-S64"
target triple = "armv7-none-linux-gnueabi"

; Function Attrs: norecurse nounwind readnone
define <4 x i32> @increment4(<4 x i32> %in) local_unnamed_addr #0 {
entry:
  %add = add <4 x i32> %in, <i32 1, i32 1, i32 1, i32 1>
  ret <4 x i32> %add
}

; Function Attrs: norecurse nounwind readnone
define <4 x i32> @uincrement4(<4 x i32> %in) local_unnamed_addr #0 {
entry:
  %add = add <4 x i32> %in, <i32 1, i32 1, i32 1, i32 1>
  ret <4 x i32> %add
}

; Function Attrs: norecurse nounwind readnone
define i32 @increment(i32 %in) local_unnamed_addr #0 {
entry:
  %add = add nsw i32 %in, 1
  ret i32 %add
}

; Function Attrs: norecurse nounwind readnone
define i32 @uincrement(i32 %in) local_unnamed_addr #0 {
entry:
  %add = add i32 %in, 1
  ret i32 %add
}

attributes #0 = { norecurse nounwind readnone "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="0" "stackrealign" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}
!\23pragma = !{!3, !4}
!\23rs_export_foreach_name = !{!5, !6, !7, !8, !9}
!\23rs_export_foreach = !{!10, !11, !11, !11, !11}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"min_enum_size", i32 4}
!2 = !{!"Android clang version 3.8.275480  (based on LLVM 3.8.275480)"}
!3 = !{!"version", !"1"}
!4 = !{!"java_package_name", !"com.android.rs.test"}
!5 = !{!"root"}
!6 = !{!"increment4"}
!7 = !{!"uincrement4"}
!8 = !{!"increment"}
!9 = !{!"uincrement"}
!10 = !{!"0"}
!11 = !{!"35"}
