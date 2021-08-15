; Expecting validation failure
; RUN: not rs2spirv_lit_driver.sh %s

target datalayout = "e-m:e-i64:64-i128:128-n32:64-S128"
target triple = "aarch64-none-linux-gnueabi"

; Function Attrs: nounwind readnone
; CHECK: OpEntryPoint GLCompute %{{[a-zA-Z_0-9]*}} "entry_fib" %gl_GlobalInvocationID
define i32 @fib(i32 %n) #0 {
entry:
  %n.off8 = add i32 %n, -1
  %0 = icmp ult i32 %n.off8, 2
  br i1 %0, label %return, label %if.end

if.end:                                           ; preds = %entry, %if.end
  %n.tr10 = phi i32 [ %sub2, %if.end ], [ %n, %entry ]
  %accumulator.tr9 = phi i32 [ %add, %if.end ], [ 1, %entry ]
  %sub = add nsw i32 %n.tr10, -1
  %call = tail call i32 @fib(i32 %sub)
  %sub2 = add nsw i32 %n.tr10, -2
  %add = add nsw i32 %call, %accumulator.tr9
  %n.off = add i32 %n.tr10, -3
  %1 = icmp ult i32 %n.off, 2
  br i1 %1, label %return, label %if.end

return:                                           ; preds = %if.end, %entry
  %accumulator.tr.lcssa = phi i32 [ 1, %entry ], [ %add, %if.end ]
  ret i32 %accumulator.tr.lcssa
}

attributes #0 = { nounwind readnone "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="0" "stackrealign" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}
!\23pragma = !{!1, !2, !3}
!\23rs_export_foreach_name = !{!4, !5}
!\23rs_export_foreach = !{!6, !7}

!0 = !{!"Android clang version 3.8.256229  (based on LLVM 3.8.256229)"}
!1 = !{!"version", !"1"}
!2 = !{!"java_package_name", !"rs2spirv"}
!3 = !{!"rs_fp_relaxed", !""}
!4 = !{!"root"}
!5 = !{!"fib"}
!6 = !{!"0"}
!7 = !{!"35"}
