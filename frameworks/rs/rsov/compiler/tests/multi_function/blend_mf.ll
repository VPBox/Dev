; RUN: rs2spirv_lit_driver.sh %s | FileCheck %s

target datalayout = "e-p:32:32-i64:64-v128:64:128-n32-S64"
target triple = "armv7-none-linux-gnueabi"

; CHECK: OpEntryPoint GLCompute %[[WrapperId:[a-zA-Z_0-9]*]] "entry_setImageAlpha"

; Function Attrs: norecurse nounwind readnone
define void @NOP() #0 {
entry:
  ret void
}

; CHECK: [[KernelId:%[a-zA-Z_0-9]+]] = OpFunction {{.*}}

; Function calls to _Z14convert_uchar4Dv4_j should be translated into OpUConvert instructions
; CHECK-NOT: OpFunctionCall %v4uchar %_Z14convert_uchar4Dv4_j
; CHECK-NOT: OpFunctionCall %v4uint %_Z13convert_uint4Dv4_h
; CHECK: OpUConvert %v4uint
; CHECK: OpUConvert %v4uchar

; Function Attrs: nounwind readnone
define <4 x i8> @setImageAlpha(<4 x i8> %in) #1 {
entry:
  %call = tail call <4 x i32> @_Z13convert_uint4Dv4_h(<4 x i8> %in) #2
  %mul = mul <4 x i32> %call, <i32 37, i32 37, i32 37, i32 37>
  %shr = lshr <4 x i32> %mul, <i32 8, i32 8, i32 8, i32 8>
  %call1 = tail call <4 x i8> @_Z14convert_uchar4Dv4_j(<4 x i32> %shr) #2
  %call2 = tail call fastcc <4 x i8> @twice(<4 x i8> %call1)
  %0 = insertelement <4 x i8> %call2, i8 37, i32 3
  ret <4 x i8> %0
}

; Function Attrs: norecurse nounwind readnone
define internal fastcc <4 x i8> @twice(<4 x i8> %in) #0 {
entry:
  %mul = shl <4 x i8> %in, <i8 1, i8 1, i8 1, i8 1>
  ret <4 x i8> %mul
}

; Function Attrs: nounwind readnone
declare <4 x i8> @_Z14convert_uchar4Dv4_j(<4 x i32>) #1

; Function Attrs: nounwind readnone
declare <4 x i32> @_Z13convert_uint4Dv4_h(<4 x i8>) #1

attributes #0 = { norecurse nounwind readnone "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="0" "stackrealign" "target-features"="+long64" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="0" "stackrealign" "target-features"="+long64" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind readnone }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}
!\23pragma = !{!3, !4, !5}
!\23rs_export_func = !{!6}
!\23rs_export_foreach_name = !{!7, !8}
!\23rs_export_foreach = !{!9, !10}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"min_enum_size", i32 4}
!2 = !{!"Android clang version 3.8.256229  (based on LLVM 3.8.256229)"}
!3 = !{!"version", !"1"}
!4 = !{!"java_package_name", !"rs2spirv"}
!5 = !{!"rs_fp_relaxed", !""}
!6 = !{!"NOP"}
!7 = !{!"root"}
!8 = !{!"setImageAlpha"}
!9 = !{!"0"}
!10 = !{!"35"}
