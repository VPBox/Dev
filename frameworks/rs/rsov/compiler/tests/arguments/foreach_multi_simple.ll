; RUN: rs2spirv_lit_driver.sh %s | FileCheck %s

; Decorations
; CHECK: OpDecorate %[[BUF_0S:.*]] BufferBlock
; CHECK: OpDecorate %[[BUF_VAR0:.*]] Binding 3
; CHECK: OpDecorate %[[BUF_1S:.*]] BufferBlock
; CHECK: OpDecorate %[[BUF_VAR1:.*]] Binding 4

; CHECK:%[[BUF_PTR_TY0:.*]] = OpTypePointer Uniform %[[BUF_0S]]
; CHECK:%[[BUF_VAR0]] = OpVariable %[[BUF_PTR_TY0]] Uniform
; CHECK:%[[BUF_PTR_TY1:.*]] = OpTypePointer Uniform %[[BUF_1S]]
; CHECK:%[[BUF_VAR1]] = OpVariable %[[BUF_PTR_TY1]] Uniform

; Wrapper
; CHECK: %[[ADDR0:.*]] = OpAccessChain %{{.*}} %[[BUF_VAR0]]
; CHECK:OpLoad {{.*}} %[[ADDR0]]
; CHECK: %[[ADDR1:.*]] = OpAccessChain %{{.*}} %[[BUF_VAR1]]
; CHECK:OpLoad {{.*}} %[[ADDR1]]

target datalayout = "e-p:32:32-i64:64-v128:64:128-n32-S64"
target triple = "armv7-none-linux-gnueabi"

; Function Attrs: norecurse nounwind readnone
define float @sum2(float %in0, float %in1, i32 %x) local_unnamed_addr #0 {
entry:
  %add = fadd float %in0, %in1
  ret float %add
}

attributes #0 = { norecurse nounwind readnone "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="0" "stackrealign" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}
!\23pragma = !{!3, !4}
!\23rs_export_foreach_name = !{!5, !6}
!\23rs_export_foreach = !{!7, !8}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"min_enum_size", i32 4}
!2 = !{!"Android clang version 3.8.275480  (based on LLVM 3.8.275480)"}
!3 = !{!"version", !"1"}
!4 = !{!"java_package_name", !"com.android.rs.rsov.test"}
!5 = !{!"root"}
!6 = !{!"sum2"}
!7 = !{!"0"}
!8 = !{!"43"}
