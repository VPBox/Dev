; RUN: rs2spirv_lit_driver.sh %s | FileCheck %s

target datalayout = "e-p:32:32-i64:64-v128:64:128-n32-S64"
target triple = "armv7-none-linux-gnueabi"

; CHECK: OpEntryPoint GLCompute [[WrapperId:%[a-zA-Z_0-9]*]] "entry_setImageAlpha"

; CHECK-NOT: OpFunctionCall %v4uchar %__Z14convert_uchar4Dv4_j
; CHECK-NOT: OpFunctionCall %v4uint %__Z13convert_uint4Dv4_h

; CHECK: OpUConvert %v4uint
; CHECK: OpUConvert %v4uchar

@alpha = global i8 0, align 1

; Function Attrs: nounwind readonly
define <4 x i8> @setImageAlpha(<4 x i8> %in, i32 %x, i32 %y) #0 {
entry:
  %call = tail call <4 x i32> @_Z13convert_uint4Dv4_h(<4 x i8> %in) #2
  %0 = load i8, i8* @alpha, align 1, !tbaa !11
  %conv = zext i8 %0 to i32
  %splat.splatinsert = insertelement <4 x i32> undef, i32 %conv, i32 0
  %splat.splat = shufflevector <4 x i32> %splat.splatinsert, <4 x i32> undef, <4 x i32> zeroinitializer
  %mul = mul <4 x i32> %splat.splat, %call
  %shr = lshr <4 x i32> %mul, <i32 8, i32 8, i32 8, i32 8>
  %call1 = tail call <4 x i8> @_Z14convert_uchar4Dv4_j(<4 x i32> %shr) #2
  %1 = insertelement <4 x i8> %call1, i8 %0, i32 3
  ret <4 x i8> %1
}

; Function Attrs: nounwind readnone
declare <4 x i8> @_Z14convert_uchar4Dv4_j(<4 x i32>) #1

; Function Attrs: nounwind readnone
declare <4 x i32> @_Z13convert_uint4Dv4_h(<4 x i8>) #1

attributes #0 = { nounwind readonly "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="0" "stackrealign" "target-features"="+long64" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="0" "stackrealign" "target-features"="+long64" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind readnone }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}
!\23pragma = !{!3, !4, !5}
!\23rs_export_var = !{!6}
!\23rs_object_slots = !{}
!\23rs_export_foreach_name = !{!7, !8}
!\23rs_export_foreach = !{!9, !10}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"min_enum_size", i32 4}
!2 = !{!"Android clang version 3.8.256229  (based on LLVM 3.8.256229)"}
!3 = !{!"version", !"1"}
!4 = !{!"java_package_name", !"rs2spirv"}
!5 = !{!"rs_fp_relaxed", !""}
!6 = !{!"alpha", !"7"}
!7 = !{!"root"}
!8 = !{!"setImageAlpha"}
!9 = !{!"0"}
!10 = !{!"59"}
!11 = !{!12, !12, i64 0}
!12 = !{!"omnipotent char", !13, i64 0}
!13 = !{!"Simple C/C++ TBAA"}
