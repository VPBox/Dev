; Expecting validation failure
; RUN: not rs2spirv_lit_driver.sh %s

target datalayout = "e-m:e-i64:64-i128:128-n32:64-S128"
target triple = "aarch64-none-linux-gnueabi"

; CHECK: OpEntryPoint GLCompute %[[WrapperId:[a-zA-Z_0-9]*]] "entry_duff"
; CHECK: [[KernelId:%[a-zA-Z_0-9]+]] = OpFunction {{.*}}
; Function Attrs: norecurse nounwind readnone
define i32 @duff(i32 %count) #0 {
entry:
  %add = add nsw i32 %count, 7
  %div = sdiv i32 %add, 8
  %rem = srem i32 %count, 8
  switch i32 %rem, label %sw.epilog [
    i32 0, label %do.body
    i32 7, label %sw.bb1
    i32 6, label %sw.bb2
    i32 5, label %sw.bb3
    i32 4, label %sw.bb6
    i32 3, label %sw.bb8
    i32 2, label %sw.bb12
    i32 1, label %sw.bb14
  ]

do.body:                                          ; preds = %entry, %sw.bb14
  %n.0 = phi i32 [ %dec15, %sw.bb14 ], [ %div, %entry ]
  %x.0 = phi i32 [ %shl, %sw.bb14 ], [ 321, %entry ]
  %mul = mul nsw i32 %x.0, 5
  br label %sw.bb1

sw.bb1:                                           ; preds = %entry, %do.body
  %n.1 = phi i32 [ %n.0, %do.body ], [ %div, %entry ]
  %x.1 = phi i32 [ %mul, %do.body ], [ 321, %entry ]
  %dec = add nsw i32 %x.1, -1
  br label %sw.bb2

sw.bb2:                                           ; preds = %entry, %sw.bb1
  %n.2 = phi i32 [ %n.1, %sw.bb1 ], [ %div, %entry ]
  %x.2 = phi i32 [ %dec, %sw.bb1 ], [ 321, %entry ]
  %xor = xor i32 %x.2, 27
  br label %sw.bb3

sw.bb3:                                           ; preds = %entry, %sw.bb2
  %n.3 = phi i32 [ %n.2, %sw.bb2 ], [ %div, %entry ]
  %x.3 = phi i32 [ %xor, %sw.bb2 ], [ 321, %entry ]
  %mul4 = mul nsw i32 %x.3, %x.3
  %sub.neg = add i32 %x.3, 12
  %sub5 = sub i32 %sub.neg, %mul4
  br label %sw.bb6

sw.bb6:                                           ; preds = %entry, %sw.bb3
  %n.4 = phi i32 [ %n.3, %sw.bb3 ], [ %div, %entry ]
  %x.4 = phi i32 [ %sub5, %sw.bb3 ], [ 321, %entry ]
  %add7 = add nsw i32 %x.4, 2
  br label %sw.bb8

sw.bb8:                                           ; preds = %entry, %sw.bb6
  %n.5 = phi i32 [ %n.4, %sw.bb6 ], [ %div, %entry ]
  %x.5 = phi i32 [ %add7, %sw.bb6 ], [ 321, %entry ]
  %rem9 = srem i32 %x.5, 32
  %mul10 = mul nsw i32 %x.5, %x.5
  %add11 = add nsw i32 %rem9, %mul10
  br label %sw.bb12

sw.bb12:                                          ; preds = %entry, %sw.bb8
  %n.6 = phi i32 [ %n.5, %sw.bb8 ], [ %div, %entry ]
  %x.6 = phi i32 [ %add11, %sw.bb8 ], [ 321, %entry ]
  %sub13 = add nsw i32 %x.6, -2
  br label %sw.bb14

sw.bb14:                                          ; preds = %entry, %sw.bb12
  %n.7 = phi i32 [ %div, %entry ], [ %n.6, %sw.bb12 ]
  %x.7 = phi i32 [ 321, %entry ], [ %sub13, %sw.bb12 ]
  %shl = shl i32 %x.7, 3
  %dec15 = add nsw i32 %n.7, -1
  %cmp = icmp sgt i32 %n.7, 1
  br i1 %cmp, label %do.body, label %sw.epilog

sw.epilog:                                        ; preds = %sw.bb14, %entry
  %x.8 = phi i32 [ 321, %entry ], [ %shl, %sw.bb14 ]
  ret i32 %x.8
}

; CHECK: %[[WrapperId]] = OpFunction {{.*}}
; CHECK-NEXT: OpLabel
; CHECK: %{{[0-9]+}} = OpFunctionCall %{{.*}} [[KernelId]]
; CHECK: OpReturn

attributes #0 = { norecurse nounwind readnone "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="0" "stackrealign" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}
!\23pragma = !{!1, !2, !3}
!\23rs_export_foreach_name = !{!4, !5}
!\23rs_export_foreach = !{!6, !7}

!0 = !{!"Android clang version 3.8.256229  (based on LLVM 3.8.256229)"}
!1 = !{!"version", !"1"}
!2 = !{!"java_package_name", !"rs2spirv"}
!3 = !{!"rs_fp_relaxed", !""}
!4 = !{!"root"}
!5 = !{!"duff"}
!6 = !{!"0"}
!7 = !{!"35"}
