; This checks that RSForEachExpand generates getelementptr
; instructions into the driver info structure as expected - namely,
; that they index into the right positions of the structure and that
; the instructions that are generated are in the loop header.

; RUN: opt -load libbcc.so -kernelexp -S < %s | FileCheck %s

; ModuleID = 'test_getelementptr.bc'
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

; Old-style kernel
define void @root(i32* nocapture %ain, i32* nocapture %out, i32 %x, i32 %y, i32 %z) {
  ret void
; CHECK: define void @root.expand(%RsExpandKernelDriverInfoPfx* %p, i32 %x1, i32 %x2, i32 %outstep)
; CHECK: Begin:
; CHECK: %instep_addr.gep = getelementptr inbounds %RsExpandKernelDriverInfoPfx, %RsExpandKernelDriverInfoPfx* %p, i32 0, i32 1, i32 0
; CHECK: load i32, i32* %instep_addr.gep
; CHECK: %input_buf.gep = getelementptr inbounds %RsExpandKernelDriverInfoPfx, %RsExpandKernelDriverInfoPfx* %p, i32 0, i32 0, i32 0
; CHECK: load i8*, i8** %input_buf.gep
; CHECK: %out_buf.gep = getelementptr inbounds %RsExpandKernelDriverInfoPfx, %RsExpandKernelDriverInfoPfx* %p, i32 0, i32 3, i32 0
; CHECK: load i8*, i8** %out_buf.gep
; CHECK: %Y.gep = getelementptr inbounds %RsExpandKernelDriverInfoPfx, %RsExpandKernelDriverInfoPfx* %p, i32 0, i32 7, i32 1
; CHECK: load i32, i32* %Y.gep
; CHECK: %Z.gep = getelementptr inbounds %RsExpandKernelDriverInfoPfx, %RsExpandKernelDriverInfoPfx* %p, i32 0, i32 7, i32 2
; CHECK: load i32, i32* %Z.gep
; CHECK: Loop:
}

; New style kernel with multiple inputs
define i32 @foo(i32 %in0, i32 %in1, i32 %x, i32 %y, i32 %z) {
  ret i32 0
; CHECK: define void @foo.expand(%RsExpandKernelDriverInfoPfx* %p, i32 %x1, i32 %x2, i32 %arg_outstep)
; CHECK: Begin:
; CHECK: %out_buf.gep = getelementptr inbounds %RsExpandKernelDriverInfoPfx, %RsExpandKernelDriverInfoPfx* %p, i32 0, i32 3, i32 0
; CHECK: load i8*, i8** %out_buf.gep
; CHECK: %Y.gep = getelementptr inbounds %RsExpandKernelDriverInfoPfx, %RsExpandKernelDriverInfoPfx* %p, i32 0, i32 7, i32 1
; CHECK: load i32, i32* %Y.gep
; CHECK: %Z.gep = getelementptr inbounds %RsExpandKernelDriverInfoPfx, %RsExpandKernelDriverInfoPfx* %p, i32 0, i32 7, i32 2
; CHECK: load i32, i32* %Z.gep
; CHECK: %input_buf.gep = getelementptr inbounds %RsExpandKernelDriverInfoPfx, %RsExpandKernelDriverInfoPfx* %p, i32 0, i32 0, i32 0
; CHECK: load i8*, i8** %input_buf.gep
; CHECK: %input_buf.gep1 = getelementptr inbounds %RsExpandKernelDriverInfoPfx, %RsExpandKernelDriverInfoPfx* %p, i32 0, i32 0, i32 1
; CHECK: load i8*, i8** %input_buf.gep1
; CHECK: Loop:
}

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
!4 = !{!"foo"}
!5 = !{!"91"}
!6 = !{!"123"}
!7 = !{!"0", !"3"}
