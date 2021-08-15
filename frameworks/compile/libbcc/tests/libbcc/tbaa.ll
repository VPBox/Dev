; Basic test of TBAA that should report that pointer loads do not
; alias with stores to allocations.

; RUN: opt -load libbcc.so -kernelexp -tbaa -aa-eval -print-no-aliases -evaluate-aa-metadata < %s -S -o - 2>&1 | FileCheck %s

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

; CHECK:   NoAlias:   %0 = load {{.*}}, i8** %out_buf.gep, !tbaa {{.*}} <->   store i32 %call.result, i32* {{.*}}, !tbaa {{.*}}
; CHECK:   NoAlias:   %input_buf = load i8*, i8** %input_buf.gep, !tbaa {{.*}} <->   store i32 %call.result, i32* {{.*}}, !tbaa {{.*}}

; Function Attrs: nounwind readnone
define i32 @add1(i32 %in) #0 {
  %1 = add nsw i32 %in, 1
  ret i32 %1
}

attributes #0 = { nounwind readnone }

!llvm.ident = !{!0}
!\23pragma = !{!1, !2}
!\23rs_export_foreach_name = !{!3, !4}
!\23rs_export_foreach = !{!5, !6}

; The following named metadata would not be present in a bitcode file,
; but instead synthesized by bcc from the bitcode wrapper.  However,
; for this test case, we're not running bcc, but instead opt, and so
; we never get the opportunity to synthesize this named metadata.
!\23rs_wrapper = !{!7}

!0 = !{!"clang version 3.6 "}
!1 = !{!"version", !"1"}
!2 = !{!"java_package_name", !"foo"}
!3 = !{!"root"}
!4 = !{!"add1"}
!5 = !{!"0"}
!6 = !{!"35"}
!7 = !{!"0", !"3"}
