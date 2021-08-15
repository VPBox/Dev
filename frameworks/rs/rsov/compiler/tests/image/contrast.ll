; RUN: rs2spirv_lit_driver.sh %s | FileCheck %s

target datalayout = "e-p:32:32-i64:64-v128:64:128-n32-S64"
target triple = "armv7-none-linux-gnueabi"

; CHECK: [[GLSL_EXT_INS:%.*]] = OpExtInstImport "GLSL.std.450"
; CHECK: OpEntryPoint GLCompute [[WrapperId:%[a-zA-Z_0-9]*]] "entry_contrast"

; CHECK-NOT: OpFunctionCall %v3float %_Z14convert_float3Dv3_h
; CHECK-NOT: OpFunctionCall %v3float %_Z5clampDv3_fff
; CHECK-NOT: OpFunctionCall %v3uchar %_Z14convert_uchar3Dv3_f

; CHECK: OpConvertUToF %v3float
; CHECK: OpExtInst %v3float [[GLSL_EXT_INS]] FClamp
; CHECK: OpConvertFToU %v3uchar


@brightM = internal unnamed_addr global float 0.000000e+00, align 4
@brightC = internal unnamed_addr global float 0.000000e+00, align 4

; Function Attrs: nounwind
define void @setBright(float %v) #0 {
entry:
  %div = fdiv float %v, 1.000000e+02
  %call = tail call float @_Z3powff(float 2.000000e+00, float %div) #4
  store float %call, float* @brightM, align 4, !tbaa !11
  %mul = fmul float %call, 1.270000e+02
  %sub = fsub float 1.270000e+02, %mul
  store float %sub, float* @brightC, align 4, !tbaa !11
  ret void
}

; Function Attrs: nounwind readnone
declare float @_Z3powff(float, float) #1

; Function Attrs: nounwind readonly
define <4 x i8> @contrast(<4 x i8> %in) #2 {
entry:
  %0 = shufflevector <4 x i8> %in, <4 x i8> undef, <3 x i32> <i32 0, i32 1, i32 2>
  %call = tail call <3 x float> @_Z14convert_float3Dv3_h(<3 x i8> %0) #4
  %1 = load float, float* @brightM, align 4, !tbaa !11
  %splat.splatinsert = insertelement <3 x float> undef, float %1, i32 0
  %splat.splat = shufflevector <3 x float> %splat.splatinsert, <3 x float> undef, <3 x i32> zeroinitializer
  %mul = fmul <3 x float> %call, %splat.splat
  %2 = load float, float* @brightC, align 4, !tbaa !11
  %splat.splatinsert1 = insertelement <3 x float> undef, float %2, i32 0
  %splat.splat2 = shufflevector <3 x float> %splat.splatinsert1, <3 x float> undef, <3 x i32> zeroinitializer
  %add = fadd <3 x float> %mul, %splat.splat2
  %call4 = tail call <3 x float> @_Z5clampDv3_fff(<3 x float> %add, float 0.000000e+00, float 2.550000e+02) #4
  %call5 = tail call <3 x i8> @_Z14convert_uchar3Dv3_f(<3 x float> %call4) #4
  %3 = shufflevector <3 x i8> %call5, <3 x i8> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 undef>
  %4 = insertelement <4 x i8> %3, i8 -1, i32 3
  ret <4 x i8> %4
}

; Function Attrs: nounwind readnone
declare <3 x float> @_Z14convert_float3Dv3_h(<3 x i8>) #1

; Function Attrs: nounwind readnone
declare <3 x i8> @_Z14convert_uchar3Dv3_f(<3 x float>) #1

; Function Attrs: nounwind readnone
declare <3 x float> @_Z5clampDv3_fff(<3 x float>, float, float) #1

; Function Attrs: noinline nounwind
define void @.helper_setBright({ float }* nocapture) #3 {
entry:
  %1 = getelementptr inbounds { float }, { float }* %0, i32 0, i32 0
  %2 = load float, float* %1, align 4
  tail call void @setBright(float %2)
  ret void
}

attributes #0 = { nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="0" "stackrealign" "target-features"="+long64" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="0" "stackrealign" "target-features"="+long64" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind readonly "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="0" "stackrealign" "target-features"="+long64" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { noinline nounwind }
attributes #4 = { nounwind readnone }

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
!6 = !{!".helper_setBright"}
!7 = !{!"root"}
!8 = !{!"contrast"}
!9 = !{!"0"}
!10 = !{!"35"}
!11 = !{!12, !12, i64 0}
!12 = !{!"float", !13, i64 0}
!13 = !{!"omnipotent char", !14, i64 0}
!14 = !{!"Simple C/C++ TBAA"}
