; This test checks that the code doesn't aggressively apply TBAA
; metadata to temporaries that are passed by pointer to kernels.

; RUN: opt -load libbcc.so -kernelexp -inline -tbaa -aa-eval -print-may-aliases -evaluate-aa-metadata < %s -S -o - 2>&1 | FileCheck %s

; ModuleID = 'kernel.bc'
target datalayout = "e-m:e-i64:64-i128:128-n32:64-S128"
target triple = "aarch64-none-linux-gnueabi"

; Declarations expected by the expansion pass.
declare void @_Z14rsGetElementAt13rs_allocationj()
declare void @_Z14rsGetElementAt13rs_allocationjj()
declare void @_Z14rsGetElementAt13rs_allocationjjj()
declare void @_Z14rsSetElementAt13rs_allocationPvj()
declare void @_Z14rsSetElementAt13rs_allocationPvjj()
declare void @_Z14rsSetElementAt13rs_allocationPvjjj()
declare void @_Z25rsGetElementAtYuv_uchar_Y13rs_allocationjj()
declare void @_Z25rsGetElementAtYuv_uchar_U13rs_allocationjj()
declare void @_Z25rsGetElementAtYuv_uchar_V13rs_allocationjj()

%struct.int5.0 = type { [5 x i32] }

; Function Attrs: nounwind
define void @add1_int5(%struct.int5.0* noalias nocapture sret %agg.result, %struct.int5.0* nocapture %in) #0 {
  br label %1

; <label>:1                                       ; preds = %1, %0
  %indvars.iv = phi i64 [ 0, %0 ], [ %indvars.iv.next, %1 ]
  %2 = getelementptr inbounds %struct.int5.0, %struct.int5.0* %in, i64 0, i32 0, i64 %indvars.iv
; CHECK: MayAlias: %load_from_input{{.*}} <-> store %struct.int5.0 %input, %struct.int5.0* %input_struct_slot
  %load_from_input = load i32, i32* %2, align 4, !tbaa !9
  %3 = add nsw i32 %load_from_input, 1
  store i32 %3, i32* %2, align 4, !tbaa !9
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond = icmp eq i64 %indvars.iv.next, 5
  br i1 %exitcond, label %4, label %1

; <label>:4                                       ; preds = %1
  %5 = bitcast %struct.int5.0* %agg.result to i8*
  %6 = bitcast %struct.int5.0* %in to i8*
  tail call void @llvm.memcpy.p0i8.p0i8.i64(i8* %5, i8* %6, i64 20, i32 4, i1 false), !tbaa.struct !13
  ret void
}

; Function Attrs: nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture, i8* nocapture readonly, i64, i32, i1) #0

attributes #0 = { nounwind }

!llvm.ident = !{!0}
!\23pragma = !{!1, !2}
!\23rs_export_foreach_name = !{!3, !4}
!\23rs_export_foreach = !{!5, !6}
!\23rs_export_type = !{!7}
!\25int5 = !{!8}

; The following named metadata would not be present in a bitcode file,
; but instead synthesized by bcc from the bitcode wrapper.  However,
; for this test case, we're not running bcc, but instead opt, and so
; we never get the opportunity to synthesize this named metadata.
!\23rs_wrapper = !{!15}

!0 = !{!"clang version 3.6 "}
!1 = !{!"version", !"1"}
!2 = !{!"java_package_name", !"foo"}
!3 = !{!"root"}
!4 = !{!"add1_int5"}
!5 = !{!"0"}
!6 = !{!"35"}
!7 = !{!"int5"}
!8 = !{!"data", !"<ConstantArray>"}
!9 = !{!10, !10, i64 0}
!10 = !{!"int", !11, i64 0}
!11 = !{!"omnipotent char", !12, i64 0}
!12 = !{!"Simple C/C++ TBAA"}
!13 = !{i64 0, i64 20, !14}
!14 = !{!11, !11, i64 0}
!15 = !{!"0", !"3"}
