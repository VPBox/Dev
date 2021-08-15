; Check that the unexpanded accumulator functions and the dummy variables are deleted.
; There doesn't seem to be any way to compute the basename of %s, hence the unfortunate
;   explicit uses of "test_reduce_general_cleanup" below.
; There doesn't seem to be a way to write a CHECK-NOT pattern that matches only at the
;   end of a line (llvm-objdump dumps symbol name at end of line), so sed is employed
;   to add a '<' at the end of each line (symbol name).  This allows us to use (e.g.)
;   "aiAccum<" to match the symbol "aiAccum" but not the symbol "aiAccum.expand".

; RUN: llvm-rs-as %s -o %t
; RUN: bcc -o test_reduce_general_cleanup -output_path %T -bclib libclcore.bc -mtriple armv7-none-linux-gnueabi %t
; RUN: llvm-objdump -t %T/test_reduce_general_cleanup.o | sed -e 's!$!<!' | FileCheck %s

; CHECK-NOT: .rs.reduce_fn
; CHECK-NOT: aiAccum<
; CHECK-NOT: mpyAccum<
; CHECK-NOT: dpAccum<
; CHECK-NOT: fMMAccumulator<
; CHECK-NOT: fzAccum<
; CHECK-NOT: fz2Accum<
; CHECK-NOT: hsgAccum<

; ModuleID = 'reduce.bc'
target datalayout = "e-p:32:32-i64:64-v128:64:128-n32-S64"
target triple = "armv7-none-linux-gnueabi"

%struct.MinAndMax = type { %struct.IndexedVal, %struct.IndexedVal }
%struct.IndexedVal = type { float, i32 }

@.rs.reduce_fn.aiAccum = global i8* bitcast (void (i32*, i32)* @aiAccum to i8*), align 4
@.rs.reduce_fn.dpAccum = global i8* bitcast (void (float*, float, float)* @dpAccum to i8*), align 4
@.rs.reduce_fn.dpSum = global i8* bitcast (void (float*, float*)* @dpSum to i8*), align 4
@.rs.reduce_fn.fMMInit = global i8* bitcast (void (%struct.MinAndMax*)* @fMMInit to i8*), align 4
@.rs.reduce_fn.fMMAccumulator = global i8* bitcast (void (%struct.MinAndMax*, float, i32)* @fMMAccumulator to i8*), align 4
@.rs.reduce_fn.fMMCombiner = global i8* bitcast (void (%struct.MinAndMax*, %struct.MinAndMax*)* @fMMCombiner to i8*), align 4
@.rs.reduce_fn.fMMOutConverter = global i8* bitcast (void (<2 x i32>*, %struct.MinAndMax*)* @fMMOutConverter to i8*), align 4
@.rs.reduce_fn.fzInit = global i8* bitcast (void (i32*)* @fzInit to i8*), align 4
@.rs.reduce_fn.fzAccum = global i8* bitcast (void (i32*, i32, i32)* @fzAccum to i8*), align 4
@.rs.reduce_fn.fzCombine = global i8* bitcast (void (i32*, i32*)* @fzCombine to i8*), align 4
@.rs.reduce_fn.fz2Init = global i8* bitcast (void (<2 x i32>*)* @fz2Init to i8*), align 4
@.rs.reduce_fn.fz2Accum = global i8* bitcast (void (<2 x i32>*, i32, i32, i32)* @fz2Accum to i8*), align 4
@.rs.reduce_fn.fz2Combine = global i8* bitcast (void (<2 x i32>*, <2 x i32>*)* @fz2Combine to i8*), align 4
@.rs.reduce_fn.fz3Init = global i8* bitcast (void (<3 x i32>*)* @fz3Init to i8*), align 4
@.rs.reduce_fn.fz3Accum = global i8* bitcast (void (<3 x i32>*, i32, i32, i32, i32)* @fz3Accum to i8*), align 4
@.rs.reduce_fn.fz3Combine = global i8* bitcast (void (<3 x i32>*, <3 x i32>*)* @fz3Combine to i8*), align 4
@.rs.reduce_fn.hsgAccum = global i8* bitcast (void ([256 x i32]*, i8)* @hsgAccum to i8*), align 4
@.rs.reduce_fn.hsgCombine = global i8* bitcast (void ([256 x i32]*, [256 x i32]*)* @hsgCombine to i8*), align 4
@.rs.reduce_fn.modeOutConvert = global i8* bitcast (void (<2 x i32>*, [256 x i32]*)* @modeOutConvert to i8*), align 4
@negInf = common global float 0.000000e+00, align 4
@posInf = common global float 0.000000e+00, align 4

; Function Attrs: nounwind
define internal void @aiAccum(i32* nocapture %accum, i32 %val) #0 {
  %1 = load i32, i32* %accum, align 4, !tbaa !22
  %2 = add nsw i32 %1, %val
  store i32 %2, i32* %accum, align 4, !tbaa !22
  ret void
}

; Function Attrs: nounwind
define internal void @dpAccum(float* nocapture %accum, float %in1, float %in2) #0 {
  %1 = fmul float %in1, %in2
  %2 = load float, float* %accum, align 4, !tbaa !26
  %3 = fadd float %1, %2
  store float %3, float* %accum, align 4, !tbaa !26
  ret void
}

; Function Attrs: nounwind
define internal void @dpSum(float* nocapture %accum, float* nocapture %val) #0 {
  %1 = load float, float* %val, align 4, !tbaa !26
  %2 = load float, float* %accum, align 4, !tbaa !26
  %3 = fadd float %1, %2
  store float %3, float* %accum, align 4, !tbaa !26
  ret void
}

; Function Attrs: nounwind
define internal void @fMMInit(%struct.MinAndMax* nocapture %accum) #0 {
  %1 = load i32, i32* bitcast (float* @posInf to i32*), align 4, !tbaa !26
  %2 = bitcast %struct.MinAndMax* %accum to i32*
  store i32 %1, i32* %2, align 4, !tbaa !26
  %3 = getelementptr inbounds %struct.MinAndMax, %struct.MinAndMax* %accum, i32 0, i32 0, i32 1
  store i32 -1, i32* %3, align 4, !tbaa !22
  %4 = load i32, i32* bitcast (float* @negInf to i32*), align 4, !tbaa !26
  %5 = getelementptr inbounds %struct.MinAndMax, %struct.MinAndMax* %accum, i32 0, i32 1
  %6 = bitcast %struct.IndexedVal* %5 to i32*
  store i32 %4, i32* %6, align 4, !tbaa !26
  %7 = getelementptr inbounds %struct.MinAndMax, %struct.MinAndMax* %accum, i32 0, i32 1, i32 1
  store i32 -1, i32* %7, align 4, !tbaa !22
  ret void
}

; Function Attrs: nounwind
define internal void @fMMAccumulator(%struct.MinAndMax* nocapture %accum, float %in, i32 %x) #0 {
  %1 = getelementptr inbounds %struct.MinAndMax, %struct.MinAndMax* %accum, i32 0, i32 0, i32 0
  %2 = load float, float* %1, align 4, !tbaa !26
  %3 = fcmp ogt float %2, %in
  br i1 %3, label %4, label %6

; <label>:4                                       ; preds = %0
  store float %in, float* %1, align 4
  %5 = getelementptr inbounds %struct.MinAndMax, %struct.MinAndMax* %accum, i32 0, i32 0, i32 1
  store i32 %x, i32* %5, align 4
  br label %6

; <label>:6                                       ; preds = %4, %0
  %7 = getelementptr inbounds %struct.MinAndMax, %struct.MinAndMax* %accum, i32 0, i32 1, i32 0
  %8 = load float, float* %7, align 4, !tbaa !26
  %9 = fcmp olt float %8, %in
  br i1 %9, label %10, label %12

; <label>:10                                      ; preds = %6
  store float %in, float* %7, align 4
  %11 = getelementptr inbounds %struct.MinAndMax, %struct.MinAndMax* %accum, i32 0, i32 1, i32 1
  store i32 %x, i32* %11, align 4
  br label %12

; <label>:12                                      ; preds = %10, %6
  ret void
}

; Function Attrs: nounwind
define internal void @fMMCombiner(%struct.MinAndMax* nocapture %accum, %struct.MinAndMax* nocapture %val) #0 {
  %1 = getelementptr inbounds %struct.MinAndMax, %struct.MinAndMax* %val, i32 0, i32 0, i32 0
  %2 = load float, float* %1, align 4, !tbaa !26
  %3 = getelementptr inbounds %struct.MinAndMax, %struct.MinAndMax* %val, i32 0, i32 0, i32 1
  %4 = load i32, i32* %3, align 4, !tbaa !22
  tail call void @fMMAccumulator(%struct.MinAndMax* %accum, float %2, i32 %4)
  %5 = getelementptr inbounds %struct.MinAndMax, %struct.MinAndMax* %val, i32 0, i32 1, i32 0
  %6 = load float, float* %5, align 4, !tbaa !26
  %7 = getelementptr inbounds %struct.MinAndMax, %struct.MinAndMax* %val, i32 0, i32 1, i32 1
  %8 = load i32, i32* %7, align 4, !tbaa !22
  tail call void @fMMAccumulator(%struct.MinAndMax* %accum, float %6, i32 %8)
  ret void
}

; Function Attrs: nounwind
define internal void @fMMOutConverter(<2 x i32>* nocapture %result, %struct.MinAndMax* nocapture %val) #0 {
  %1 = getelementptr inbounds %struct.MinAndMax, %struct.MinAndMax* %val, i32 0, i32 0, i32 1
  %2 = load i32, i32* %1, align 4, !tbaa !22
  %3 = load <2 x i32>, <2 x i32>* %result, align 8
  %4 = insertelement <2 x i32> %3, i32 %2, i32 0
  store <2 x i32> %4, <2 x i32>* %result, align 8
  %5 = getelementptr inbounds %struct.MinAndMax, %struct.MinAndMax* %val, i32 0, i32 1, i32 1
  %6 = load i32, i32* %5, align 4, !tbaa !22
  %7 = insertelement <2 x i32> %4, i32 %6, i32 1
  store <2 x i32> %7, <2 x i32>* %result, align 8
  ret void
}

; Function Attrs: nounwind
define internal void @fzInit(i32* nocapture %accumIdx) #0 {
  store i32 -1, i32* %accumIdx, align 4, !tbaa !22
  ret void
}

; Function Attrs: nounwind
define internal void @fzAccum(i32* nocapture %accumIdx, i32 %inVal, i32 %x) #0 {
  %1 = icmp eq i32 %inVal, 0
  br i1 %1, label %2, label %3

; <label>:2                                       ; preds = %0
  store i32 %x, i32* %accumIdx, align 4, !tbaa !22
  br label %3

; <label>:3                                       ; preds = %2, %0
  ret void
}

; Function Attrs: nounwind
define internal void @fzCombine(i32* nocapture %accumIdx, i32* nocapture %accumIdx2) #0 {
  %1 = load i32, i32* %accumIdx2, align 4, !tbaa !22
  %2 = icmp sgt i32 %1, -1
  br i1 %2, label %3, label %4

; <label>:3                                       ; preds = %0
  store i32 %1, i32* %accumIdx, align 4, !tbaa !22
  br label %4

; <label>:4                                       ; preds = %3, %0
  ret void
}

; Function Attrs: nounwind
define internal void @fz2Init(<2 x i32>* nocapture %accum) #0 {
  store <2 x i32> <i32 -1, i32 -1>, <2 x i32>* %accum, align 8
  ret void
}

; Function Attrs: nounwind
define internal void @fz2Accum(<2 x i32>* nocapture %accum, i32 %inVal, i32 %x, i32 %y) #0 {
  %1 = icmp eq i32 %inVal, 0
  br i1 %1, label %2, label %5

; <label>:2                                       ; preds = %0
  %3 = insertelement <2 x i32> undef, i32 %x, i32 0
  %4 = insertelement <2 x i32> %3, i32 %y, i32 1
  store <2 x i32> %4, <2 x i32>* %accum, align 8
  br label %5

; <label>:5                                       ; preds = %2, %0
  ret void
}

; Function Attrs: nounwind
define internal void @fz2Combine(<2 x i32>* nocapture %accum, <2 x i32>* nocapture %accum2) #0 {
  %1 = load <2 x i32>, <2 x i32>* %accum2, align 8
  %2 = extractelement <2 x i32> %1, i32 0
  %3 = icmp sgt i32 %2, -1
  br i1 %3, label %4, label %5

; <label>:4                                       ; preds = %0
  store <2 x i32> %1, <2 x i32>* %accum, align 8, !tbaa !28
  br label %5

; <label>:5                                       ; preds = %4, %0
  ret void
}

; Function Attrs: nounwind
define internal void @fz3Init(<3 x i32>* nocapture %accum) #0 {
  store <3 x i32> <i32 -1, i32 -1, i32 -1>, <3 x i32>* %accum, align 16
  ret void
}

; Function Attrs: nounwind
define internal void @fz3Accum(<3 x i32>* nocapture %accum, i32 %inVal, i32 %x, i32 %y, i32 %z) #0 {
  %1 = icmp eq i32 %inVal, 0
  br i1 %1, label %2, label %6

; <label>:2                                       ; preds = %0
  %3 = insertelement <3 x i32> undef, i32 %x, i32 0
  %4 = insertelement <3 x i32> %3, i32 %y, i32 1
  %5 = insertelement <3 x i32> %4, i32 %z, i32 2
  store <3 x i32> %5, <3 x i32>* %accum, align 16
  br label %6

; <label>:6                                       ; preds = %2, %0
  ret void
}

; Function Attrs: nounwind
define internal void @fz3Combine(<3 x i32>* nocapture %accum, <3 x i32>* nocapture %accum2) #0 {
  %1 = load <3 x i32>, <3 x i32>* %accum, align 16
  %2 = extractelement <3 x i32> %1, i32 0
  %3 = icmp sgt i32 %2, -1
  br i1 %3, label %4, label %8

; <label>:4                                       ; preds = %0
  %5 = bitcast <3 x i32>* %accum2 to <4 x i32>*
  %6 = load <4 x i32>, <4 x i32>* %5, align 8
  %7 = bitcast <3 x i32>* %accum to <4 x i32>*
  store <4 x i32> %6, <4 x i32>* %7, align 16, !tbaa !28
  br label %8

; <label>:8                                       ; preds = %4, %0
  ret void
}

; Function Attrs: nounwind
define internal void @hsgAccum([256 x i32]* nocapture %h, i8 zeroext %in) #0 {
  %1 = zext i8 %in to i32
  %2 = getelementptr inbounds [256 x i32], [256 x i32]* %h, i32 0, i32 %1
  %3 = load i32, i32* %2, align 4, !tbaa !22
  %4 = add i32 %3, 1
  store i32 %4, i32* %2, align 4, !tbaa !22
  ret void
}

; Function Attrs: nounwind
define internal void @hsgCombine([256 x i32]* nocapture %accum, [256 x i32]* nocapture %addend) #0 {
  br label %2

; <label>:1                                       ; preds = %2
  ret void

; <label>:2                                       ; preds = %2, %0
  %i.01 = phi i32 [ 0, %0 ], [ %8, %2 ]
  %3 = getelementptr inbounds [256 x i32], [256 x i32]* %addend, i32 0, i32 %i.01
  %4 = load i32, i32* %3, align 4, !tbaa !22
  %5 = getelementptr inbounds [256 x i32], [256 x i32]* %accum, i32 0, i32 %i.01
  %6 = load i32, i32* %5, align 4, !tbaa !22
  %7 = add i32 %6, %4
  store i32 %7, i32* %5, align 4, !tbaa !22
  %8 = add nuw nsw i32 %i.01, 1
  %exitcond = icmp eq i32 %8, 256
  br i1 %exitcond, label %1, label %2
}

; Function Attrs: nounwind
define internal void @modeOutConvert(<2 x i32>* nocapture %result, [256 x i32]* nocapture %h) #0 {
  br label %7

; <label>:1                                       ; preds = %7
  %2 = load <2 x i32>, <2 x i32>* %result, align 8
  %3 = insertelement <2 x i32> %2, i32 %i.0.mode.0, i32 0
  store <2 x i32> %3, <2 x i32>* %result, align 8
  %4 = getelementptr inbounds [256 x i32], [256 x i32]* %h, i32 0, i32 %i.0.mode.0
  %5 = load i32, i32* %4, align 4, !tbaa !22
  %6 = insertelement <2 x i32> %3, i32 %5, i32 1
  store <2 x i32> %6, <2 x i32>* %result, align 8
  ret void

; <label>:7                                       ; preds = %7, %0
  %i.02 = phi i32 [ 1, %0 ], [ %13, %7 ]
  %mode.01 = phi i32 [ 0, %0 ], [ %i.0.mode.0, %7 ]
  %8 = getelementptr inbounds [256 x i32], [256 x i32]* %h, i32 0, i32 %i.02
  %9 = load i32, i32* %8, align 4, !tbaa !22
  %10 = getelementptr inbounds [256 x i32], [256 x i32]* %h, i32 0, i32 %mode.01
  %11 = load i32, i32* %10, align 4, !tbaa !22
  %12 = icmp ugt i32 %9, %11
  %i.0.mode.0 = select i1 %12, i32 %i.02, i32 %mode.01
  %13 = add nuw nsw i32 %i.02, 1
  %exitcond = icmp eq i32 %13, 256
  br i1 %exitcond, label %1, label %7
}

attributes #0 = { nounwind }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}
!\23pragma = !{!3, !4}
!\23rs_export_var = !{!5, !6}
!\23rs_object_slots = !{}
!\23rs_export_reduce = !{!7, !9, !11, !13, !15, !17, !19, !21}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"min_enum_size", i32 4}
!2 = !{!"clang version 3.6 "}
!3 = !{!"version", !"1"}
!4 = !{!"java_package_name", !"com.android.rs.test"}
!5 = !{!"negInf", !"1"}
!6 = !{!"posInf", !"1"}
!7 = !{!"addint", !"4", !8}
!8 = !{!"aiAccum", !"1"}
!9 = !{!"dp", !"4", !10, null, !"dpSum"}
!10 = !{!"dpAccum", !"1"}
!11 = !{!"findMinAndMax", !"16", !12, !"fMMInit", !"fMMCombiner", !"fMMOutConverter"}
!12 = !{!"fMMAccumulator", !"9"}
!13 = !{!"fz", !"4", !14, !"fzInit", !"fzCombine"}
!14 = !{!"fzAccum", !"9"}
!15 = !{!"fz2", !"8", !16, !"fz2Init", !"fz2Combine"}
!16 = !{!"fz2Accum", !"25"}
!17 = !{!"fz3", !"16", !18, !"fz3Init", !"fz3Combine"}
!18 = !{!"fz3Accum", !"89"}
!19 = !{!"histogram", !"1024", !20, null, !"hsgCombine"}
!20 = !{!"hsgAccum", !"1"}
!21 = !{!"mode", !"1024", !20, null, !"hsgCombine", !"modeOutConvert"}
!22 = !{!23, !23, i64 0}
!23 = !{!"int", !24, i64 0}
!24 = !{!"omnipotent char", !25, i64 0}
!25 = !{!"Simple C/C++ TBAA"}
!26 = !{!27, !27, i64 0}
!27 = !{!"float", !24, i64 0}
!28 = !{!24, !24, i64 0}
