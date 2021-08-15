; RUN: rs2spirv_lit_driver.sh %s | FileCheck %s
; CHECK: OpEntryPoint GLCompute %{{[a-zA-Z_0-9]*}} "entry_inc" %gl_GlobalInvocationID
; CHECK-NOT: an_invokable

target datalayout = "e-p:32:32-i64:64-v128:64:128-n32-S64"
target triple = "armv7-none-linux-gnueabi"

@.str = private unnamed_addr constant [24 x i8] c"test_root_output FAILED\00", align 1

; Function Attrs: nounwind
define void @an_invokable(float %i) local_unnamed_addr #0 {
entry:
  %call = tail call fastcc float @sum(float %i, float 2.000000e+00)
  %conv = fptosi float %call to i32
  tail call void @_Z7rsDebugPKci(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @.str, i32 0, i32 0), i32 %conv) #4
  ret void
}

declare void @_Z7rsDebugPKci(i8*, i32) local_unnamed_addr #1

; Function Attrs: norecurse nounwind readnone
define internal fastcc float @sum(float %i, float %j) unnamed_addr #2 {
entry:
  %add = fadd float %i, %j
  ret float %add
}

; Function Attrs: norecurse nounwind readnone
define float @inc(float %i) local_unnamed_addr #2 {
entry:
  %call = tail call fastcc float @sum(float %i, float 1.000000e+00)
  ret float %call
}

; Function Attrs: noinline nounwind
define void @.helper_an_invokable({ float }* nocapture) local_unnamed_addr #3 {
entry:
  %1 = getelementptr inbounds { float }, { float }* %0, i32 0, i32 0
  %2 = load float, float* %1, align 4
  tail call void @an_invokable(float %2)
  ret void
}

attributes #0 = { nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="0" "stackrealign" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="0" "stackrealign" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { norecurse nounwind readnone "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="0" "stackrealign" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { noinline nounwind }
attributes #4 = { nounwind }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}
!\23pragma = !{!3, !4}
!\23rs_export_func = !{!5}
!\23rs_export_foreach_name = !{!6, !7}
!\23rs_export_foreach = !{!8, !9}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"min_enum_size", i32 4}
!2 = !{!"Android clang version 3.8.275480  (based on LLVM 3.8.275480)"}
!3 = !{!"version", !"1"}
!4 = !{!"java_package_name", !"rs2spirv"}
!5 = !{!".helper_an_invokable"}
!6 = !{!"root"}
!7 = !{!"inc"}
!8 = !{!"0"}
!9 = !{!"35"}
