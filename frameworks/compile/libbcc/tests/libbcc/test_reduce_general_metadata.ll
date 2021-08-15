; Check that the #rs_export_reduce node is recognized.

; RUN: llvm-rs-as %s -o %t
; RUN: bcinfo %t | FileCheck %s

; CHECK: exportReduceCount: 8
; CHECK: exportReduceList[0]: addint - 0x00000001 - 1 - 4
; CHECK:   accumulator(aiAccum)
; CHECK: exportReduceList[1]: mpyint - 0x00000001 - 1 - 4
; CHECK:   initializer(mpyInit)
; CHECK:   accumulator(mpyAccum)
; CHECK: exportReduceList[2]: dp - 0x00000001 - 2 - 4
; CHECK:   accumulator(dpAccum)
; CHECK:   combiner(dpSum)
; CHECK: exportReduceList[3]: findMinAndMax - 0x00000009 - 1 - 16
; CHECK:   initializer(fMMInit)
; CHECK:   accumulator(fMMAccumulator)
; CHECK:   combiner(fMMCombiner)
; CHECK:   outconverter(fMMOutConverter)
; CHECK: exportReduceList[4]: fz - 0x00000009 - 1 - 4
; CHECK:   initializer(fzInit)
; CHECK:   accumulator(fzAccum)
; CHECK:   combiner(fzCombine)
; CHECK:   halter(fzFound)
; CHECK: exportReduceList[5]: fz2 - 0x00000019 - 1 - 8
; CHECK:   initializer(fz2Init)
; CHECK:   accumulator(fz2Accum)
; CHECK:   combiner(fz2Combine)
; CHECK:   halter(fz2Found)
; CHECK: exportReduceList[6]: histogram - 0x00000001 - 1 - 1024
; CHECK:   accumulator(hsgAccum)
; CHECK:   combiner(hsgCombine)
; CHECK: exportReduceList[7]: mode - 0x00000001 - 1 - 1024
; CHECK:   accumulator(hsgAccum)
; CHECK:   combiner(hsgCombine)
; CHECK:   outconverter(modeOutConvert)

; ModuleID = 'reduce_general_examples.bc'
target datalayout = "e-m:e-i64:64-i128:128-n32:64-S128"
target triple = "aarch64-none-linux-gnueabi"

%struct.MinAndMax.0 = type { %struct.IndexedVal.1, %struct.IndexedVal.1 }
%struct.IndexedVal.1 = type { float, i32 }

@fMMInit.r = internal unnamed_addr constant %struct.MinAndMax.0 { %struct.IndexedVal.1 { float 0.000000e+00, i32 -1 }, %struct.IndexedVal.1 { float -0.000000e+00, i32 -1 } }, align 4
@llvm.used = appending global [20 x i8*] [i8* bitcast (void (<2 x i32>*)* @fz2Init to i8*), i8* bitcast (void ([256 x i32]*, [256 x i32]*)* @hsgCombine to i8*), i8* bitcast (i1 (<2 x i32>*)* @fz2Found to i8*), i8* bitcast (void (i32*, i32)* @mpyAccum to i8*), i8* bitcast (void (%struct.MinAndMax.0*)* @fMMInit to i8*), i8* bitcast (void (float*, float, float)* @dpAccum to i8*), i8* bitcast (void (<2 x i32>*, [256 x i32]*)* @modeOutConvert to i8*), i8* bitcast (void ([256 x i32]*, i8)* @hsgAccum to i8*), i8* bitcast (void (i32*)* @mpyInit to i8*), i8* bitcast (void (%struct.MinAndMax.0*, float, i32)* @fMMAccumulator to i8*), i8* bitcast (void (float*, float*)* @dpSum to i8*), i8* bitcast (void (%struct.MinAndMax.0*, %struct.MinAndMax.0*)* @fMMCombiner to i8*), i8* bitcast (void (i32*, i32*)* @fzCombine to i8*), i8* bitcast (void (i32*, i32)* @aiAccum to i8*), i8* bitcast (void (i32*)* @fzInit to i8*), i8* bitcast (void (i32*, i32, i32)* @fzAccum to i8*), i8* bitcast (i1 (i32*)* @fzFound to i8*), i8* bitcast (void (<2 x i32>*, i32, i32, i32)* @fz2Accum to i8*), i8* bitcast (void (<2 x i32>*, %struct.MinAndMax.0*)* @fMMOutConverter to i8*), i8* bitcast (void (<2 x i32>*, <2 x i32>*)* @fz2Combine to i8*)], section "llvm.metadata"

; Function Attrs: nounwind
define internal void @aiAccum(i32* nocapture %accum, i32 %val) #0 {
  %1 = load i32, i32* %accum, align 4, !tbaa !18
  %2 = add nsw i32 %1, %val
  store i32 %2, i32* %accum, align 4, !tbaa !18
  ret void
}

; Function Attrs: nounwind
define internal void @mpyInit(i32* nocapture %accum) #0 {
  store i32 1, i32* %accum, align 4, !tbaa !18
  ret void
}

; Function Attrs: nounwind
define internal void @mpyAccum(i32* nocapture %accum, i32 %val) #0 {
  %1 = load i32, i32* %accum, align 4, !tbaa !18
  %2 = mul nsw i32 %1, %val
  store i32 %2, i32* %accum, align 4, !tbaa !18
  ret void
}

; Function Attrs: nounwind
define internal void @dpAccum(float* nocapture %accum, float %in1, float %in2) #0 {
  %1 = fmul float %in1, %in2
  %2 = load float, float* %accum, align 4, !tbaa !22
  %3 = fadd float %1, %2
  store float %3, float* %accum, align 4, !tbaa !22
  ret void
}

; Function Attrs: nounwind
define internal void @dpSum(float* nocapture %accum, float* nocapture %val) #0 {
  %1 = load float, float* %val, align 4, !tbaa !22
  %2 = load float, float* %accum, align 4, !tbaa !22
  %3 = fadd float %1, %2
  store float %3, float* %accum, align 4, !tbaa !22
  ret void
}

; Function Attrs: nounwind
define internal void @fMMInit(%struct.MinAndMax.0* nocapture %accum) #0 {
  %1 = bitcast %struct.MinAndMax.0* %accum to i8*
  tail call void @llvm.memcpy.p0i8.p0i8.i64(i8* %1, i8* bitcast (%struct.MinAndMax.0* @fMMInit.r to i8*), i64 16, i32 4, i1 false), !tbaa.struct !24
  ret void
}

; Function Attrs: nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture, i8* nocapture readonly, i64, i32, i1) #0

; Function Attrs: nounwind
define internal void @fMMAccumulator(%struct.MinAndMax.0* nocapture %accum, float %in, i32 %x) #0 {
  %1 = getelementptr inbounds %struct.MinAndMax.0, %struct.MinAndMax.0* %accum, i64 0, i32 0, i32 0
  %2 = load float, float* %1, align 4, !tbaa !22
  %3 = fcmp ogt float %2, %in
  br i1 %3, label %4, label %6

; <label>:4                                       ; preds = %0
  store float %in, float* %1, align 4
  %5 = getelementptr inbounds %struct.MinAndMax.0, %struct.MinAndMax.0* %accum, i64 0, i32 0, i32 1
  store i32 %x, i32* %5, align 4
  br label %6

; <label>:6                                       ; preds = %4, %0
  %7 = getelementptr inbounds %struct.MinAndMax.0, %struct.MinAndMax.0* %accum, i64 0, i32 1, i32 0
  %8 = load float, float* %7, align 4, !tbaa !22
  %9 = fcmp olt float %8, %in
  br i1 %9, label %10, label %12

; <label>:10                                      ; preds = %6
  store float %in, float* %7, align 4
  %11 = getelementptr inbounds %struct.MinAndMax.0, %struct.MinAndMax.0* %accum, i64 0, i32 1, i32 1
  store i32 %x, i32* %11, align 4
  br label %12

; <label>:12                                      ; preds = %10, %6
  ret void
}

; Function Attrs: nounwind
define internal void @fMMCombiner(%struct.MinAndMax.0* nocapture %accum, %struct.MinAndMax.0* nocapture %val) #0 {
  %1 = getelementptr inbounds %struct.MinAndMax.0, %struct.MinAndMax.0* %val, i64 0, i32 0, i32 0
  %2 = load float, float* %1, align 4, !tbaa !22
  %3 = getelementptr inbounds %struct.MinAndMax.0, %struct.MinAndMax.0* %val, i64 0, i32 0, i32 1
  %4 = load i32, i32* %3, align 4, !tbaa !18
  tail call void @fMMAccumulator(%struct.MinAndMax.0* %accum, float %2, i32 %4)
  %5 = getelementptr inbounds %struct.MinAndMax.0, %struct.MinAndMax.0* %val, i64 0, i32 1, i32 0
  %6 = load float, float* %5, align 4, !tbaa !22
  %7 = getelementptr inbounds %struct.MinAndMax.0, %struct.MinAndMax.0* %val, i64 0, i32 1, i32 1
  %8 = load i32, i32* %7, align 4, !tbaa !18
  tail call void @fMMAccumulator(%struct.MinAndMax.0* %accum, float %6, i32 %8)
  ret void
}

; Function Attrs: nounwind
define internal void @fMMOutConverter(<2 x i32>* nocapture %result, %struct.MinAndMax.0* nocapture %val) #0 {
  %1 = getelementptr inbounds %struct.MinAndMax.0, %struct.MinAndMax.0* %val, i64 0, i32 0, i32 1
  %2 = load i32, i32* %1, align 4, !tbaa !18
  %3 = load <2 x i32>, <2 x i32>* %result, align 8
  %4 = insertelement <2 x i32> %3, i32 %2, i64 0
  store <2 x i32> %4, <2 x i32>* %result, align 8
  %5 = getelementptr inbounds %struct.MinAndMax.0, %struct.MinAndMax.0* %val, i64 0, i32 1, i32 1
  %6 = load i32, i32* %5, align 4, !tbaa !18
  %7 = insertelement <2 x i32> %4, i32 %6, i64 1
  store <2 x i32> %7, <2 x i32>* %result, align 8
  ret void
}

; Function Attrs: nounwind
define internal void @fzInit(i32* nocapture %accumIdx) #0 {
  store i32 -1, i32* %accumIdx, align 4, !tbaa !18
  ret void
}

; Function Attrs: nounwind
define internal void @fzAccum(i32* nocapture %accumIdx, i32 %inVal, i32 %x) #0 {
  %1 = icmp eq i32 %inVal, 0
  br i1 %1, label %2, label %3

; <label>:2                                       ; preds = %0
  store i32 %x, i32* %accumIdx, align 4, !tbaa !18
  br label %3

; <label>:3                                       ; preds = %2, %0
  ret void
}

; Function Attrs: nounwind
define internal void @fzCombine(i32* nocapture %accumIdx, i32* nocapture %accumIdx2) #0 {
  %1 = load i32, i32* %accumIdx2, align 4, !tbaa !18
  %2 = icmp sgt i32 %1, -1
  br i1 %2, label %3, label %4

; <label>:3                                       ; preds = %0
  store i32 %1, i32* %accumIdx, align 4, !tbaa !18
  br label %4

; <label>:4                                       ; preds = %3, %0
  ret void
}

; Function Attrs: nounwind readonly
define internal i1 @fzFound(i32* nocapture %accumIdx) #1 {
  %1 = load i32, i32* %accumIdx, align 4, !tbaa !18
  %2 = icmp sgt i32 %1, -1
  ret i1 %2
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
  %3 = insertelement <2 x i32> undef, i32 %x, i64 0
  %4 = insertelement <2 x i32> %3, i32 %y, i64 1
  store <2 x i32> %4, <2 x i32>* %accum, align 8
  br label %5

; <label>:5                                       ; preds = %2, %0
  ret void
}

; Function Attrs: nounwind
define internal void @fz2Combine(<2 x i32>* nocapture %accum, <2 x i32>* nocapture %accum2) #0 {
  %1 = load <2 x i32>, <2 x i32>* %accum2, align 8
  %2 = extractelement <2 x i32> %1, i64 0
  %3 = icmp sgt i32 %2, -1
  br i1 %3, label %4, label %5

; <label>:4                                       ; preds = %0
  store <2 x i32> %1, <2 x i32>* %accum, align 8, !tbaa !25
  br label %5

; <label>:5                                       ; preds = %4, %0
  ret void
}

; Function Attrs: nounwind readonly
define internal i1 @fz2Found(<2 x i32>* nocapture %accum) #1 {
  %1 = load <2 x i32>, <2 x i32>* %accum, align 8
  %2 = extractelement <2 x i32> %1, i64 0
  %3 = icmp sgt i32 %2, -1
  ret i1 %3
}

; Function Attrs: nounwind
define internal void @hsgAccum([256 x i32]* nocapture %h, i8 %in) #0 {
  %1 = zext i8 %in to i64
  %2 = getelementptr inbounds [256 x i32], [256 x i32]* %h, i64 0, i64 %1
  %3 = load i32, i32* %2, align 4, !tbaa !18
  %4 = add i32 %3, 1
  store i32 %4, i32* %2, align 4, !tbaa !18
  ret void
}

; Function Attrs: nounwind
define internal void @hsgCombine([256 x i32]* nocapture %accum, [256 x i32]* nocapture %addend) #0 {
  br label %2

; <label>:1                                       ; preds = %2
  ret void

; <label>:2                                       ; preds = %2, %0
  %indvars.iv = phi i64 [ 0, %0 ], [ %indvars.iv.next, %2 ]
  %3 = getelementptr inbounds [256 x i32], [256 x i32]* %addend, i64 0, i64 %indvars.iv
  %4 = load i32, i32* %3, align 4, !tbaa !18
  %5 = getelementptr inbounds [256 x i32], [256 x i32]* %accum, i64 0, i64 %indvars.iv
  %6 = load i32, i32* %5, align 4, !tbaa !18
  %7 = add i32 %6, %4
  store i32 %7, i32* %5, align 4, !tbaa !18
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond = icmp eq i64 %indvars.iv.next, 256
  br i1 %exitcond, label %1, label %2
}

; Function Attrs: nounwind
define internal void @modeOutConvert(<2 x i32>* nocapture %result, [256 x i32]* nocapture %h) #0 {
  br label %8

; <label>:1                                       ; preds = %8
  %2 = load <2 x i32>, <2 x i32>* %result, align 8
  %3 = insertelement <2 x i32> %2, i32 %i.0.mode.0, i64 0
  store <2 x i32> %3, <2 x i32>* %result, align 8
  %4 = zext i32 %i.0.mode.0 to i64
  %5 = getelementptr inbounds [256 x i32], [256 x i32]* %h, i64 0, i64 %4
  %6 = load i32, i32* %5, align 4, !tbaa !18
  %7 = insertelement <2 x i32> %3, i32 %6, i64 1
  store <2 x i32> %7, <2 x i32>* %result, align 8
  ret void

; <label>:8                                       ; preds = %8, %0
  %indvars.iv = phi i64 [ 1, %0 ], [ %indvars.iv.next, %8 ]
  %mode.01 = phi i32 [ 0, %0 ], [ %i.0.mode.0, %8 ]
  %9 = getelementptr inbounds [256 x i32], [256 x i32]* %h, i64 0, i64 %indvars.iv
  %10 = load i32, i32* %9, align 4, !tbaa !18
  %11 = zext i32 %mode.01 to i64
  %12 = getelementptr inbounds [256 x i32], [256 x i32]* %h, i64 0, i64 %11
  %13 = load i32, i32* %12, align 4, !tbaa !18
  %14 = icmp ugt i32 %10, %13
  %15 = trunc i64 %indvars.iv to i32
  %i.0.mode.0 = select i1 %14, i32 %15, i32 %mode.01
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond = icmp eq i64 %indvars.iv.next, 256
  br i1 %exitcond, label %1, label %8
}

attributes #0 = { nounwind }
attributes #1 = { nounwind readonly }

!llvm.ident = !{!0}
!\23pragma = !{!1, !2}
!\23rs_export_reduce = !{!3, !5, !7, !9, !11, !13, !15, !17}

!0 = !{!"clang version 3.6 "}
!1 = !{!"version", !"1"}
!2 = !{!"java_package_name", !"examples"}
!3 = !{!"addint", !"4", !4}
!4 = !{!"aiAccum", !"1"}
!5 = !{!"mpyint", !"4", !6, !"mpyInit"}
!6 = !{!"mpyAccum", !"1"}
!7 = !{!"dp", !"4", !8, null, !"dpSum"}
!8 = !{!"dpAccum", !"1"}
!9 = !{!"findMinAndMax", !"16", !10, !"fMMInit", !"fMMCombiner", !"fMMOutConverter"}
!10 = !{!"fMMAccumulator", !"9"}
!11 = !{!"fz", !"4", !12, !"fzInit", !"fzCombine", null, !"fzFound"}
!12 = !{!"fzAccum", !"9"}
!13 = !{!"fz2", !"8", !14, !"fz2Init", !"fz2Combine", null, !"fz2Found"}
!14 = !{!"fz2Accum", !"25"}
!15 = !{!"histogram", !"1024", !16, null, !"hsgCombine"}
!16 = !{!"hsgAccum", !"1"}
!17 = !{!"mode", !"1024", !16, null, !"hsgCombine", !"modeOutConvert"}
!18 = !{!19, !19, i64 0}
!19 = !{!"int", !20, i64 0}
!20 = !{!"omnipotent char", !21, i64 0}
!21 = !{!"Simple C/C++ TBAA"}
!22 = !{!23, !23, i64 0}
!23 = !{!"float", !20, i64 0}
!24 = !{i64 0, i64 4, !22, i64 4, i64 4, !18, i64 8, i64 4, !22, i64 12, i64 4, !18}
!25 = !{!20, !20, i64 0}
