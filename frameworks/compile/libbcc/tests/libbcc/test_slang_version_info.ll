; Check that bcc properly emits the respective slang and bcc llvm versions used
; to generate the source into the `.rs.info` symbol that's placed in the
; `.rodata` section of the ELF output file.

; RUN: sed s/REPLACE_LLVM_VERSION_STRING_HERE/%LLVM_VERSION_STRING/g %s \
; RUN:      | llvm-as -o %t.known-version

; synthetic version of slang that will likely not exist in the wild
; RUN: sed s/REPLACE_LLVM_VERSION_STRING_HERE/VERSIONDOESNOTEXIST/g %s \
; RUN:      | llvm-as -o %t.unknown-version


; RUN: bcc -o test_slang_version_info-known-version -output_path %T \
; RUN:      -bclib libclcore.bc -mtriple armv7-none-linux-gnueabi -O0 \
; RUN:      -embedRSInfo %t.known-version 2>%t.stderr.known-version

; RUN: bcc -o test_slang_version_info-unknown-version -output_path %T \
; RUN:      -bclib libclcore.bc -mtriple armv7-none-linux-gnueabi -O0 \
; RUN:      -embedRSInfo %t.unknown-version 2>%t.stderr.unknown-version

; Assert that bcc warns that there is a mismatch when slang version is the string "DOESNOTEXIST"
; RUN: FileCheck %s -check-prefix=CHECK_BCC_WARNING < %t.stderr.unknown-version

; Assert that bcc does not warn when there is a version match
; RUN: FileCheck %s -check-prefix=CHECK_BCC_NOWARNING < %t.stderr.known-version

; Assert that bcc emits the correct version info into the .rs.info symbol
; RUN: readelf --wide --string-dump=.rodata %T/test_slang_version_info-known-version.o \
; RUN:      | FileCheck %s -check-prefix=CHECK_KNOWN_VERSION

; Assert that bcc emits the correct version info into the .rs.info symbol
; RUN: readelf --wide --string-dump=.rodata %T/test_slang_version_info-unknown-version.o \
; RUN:      | FileCheck %s -check-prefix=CHECK_UNKNOWN_VERSION


; Finally check whether bcc is happy with a file that doesn't contain the
; version metatadata for compatibility with old versions of bitcode
; RUN: grep -v slang.llvm.version %s | llvm-as -o %t.no-version
; RUN: bcc -o test_slang_version_info-known-version -output_path %T \
; RUN:      -bclib libclcore.bc -mtriple armv7-none-linux-gnueabi -O0 \
; RUN:      -embedRSInfo %t.no-version 2> %t.stderr.no-version

; CHECK_UNKNOWN_VERSION: {{.*}}versionInfo: 2^Jbcc - {{[0-9]+.[0-9]+.[0-9]+}}^Jslang - VERSIONDOESNOTEXIST
; CHECK_KNOWN_VERSION: {{.*}}versionInfo: 2^Jbcc - {{[0-9]+.[0-9]+.[0-9]+}}^Jslang - {{[0-9]+.[0-9]+.[0-9]+}}
; CHECK_BCC_WARNING: W/bcc {{.*}}VERSIONDOESNOTEXIST, expecting {{[1-9]+.[0-9]+.[0-9]+.*}}The debugging experience may be unreliable
; CHECK_BCC_NOWARNING-NOT: W/bcc {{.*}}The debugging experience may be unreliable


; ModuleID = 'test_slang_version_info.bc'
target datalayout = "e-p:32:32-i64:64-v128:64:128-n32-S64"
target triple = "armv7-none-linux-gnueabi"

; Function Attrs: nounwind
define <4 x i8> @swizzle(<4 x i8> %in) #0 !dbg !4 {
  %1 = alloca <4 x i8>, align 4
  %result = alloca <4 x i8>, align 4
  store <4 x i8> %in, <4 x i8>* %1, align 4
  call void @llvm.dbg.declare(metadata <4 x i8>* %1, metadata !27, metadata !28), !dbg !29
  call void @llvm.dbg.declare(metadata <4 x i8>* %result, metadata !30, metadata !28), !dbg !31
  %2 = load <4 x i8>, <4 x i8>* %1, align 4, !dbg !31
  %3 = shufflevector <4 x i8> %2, <4 x i8> undef, <4 x i32> <i32 3, i32 2, i32 1, i32 0>, !dbg !31
  store <4 x i8> %3, <4 x i8>* %result, align 4, !dbg !31
  %4 = load <4 x i8>, <4 x i8>* %result, align 4, !dbg !32
  ret <4 x i8> %4, !dbg !32
}

; Function Attrs: nounwind readnone
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

attributes #0 = { nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="0" "stackrealign" "target-features"="+long64" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!16, !17, !18}
!llvm.ident = !{!19}
!\23pragma = !{!20, !21}
!\23rs_export_foreach_name = !{!22, !23}
!\23rs_export_foreach = !{!24, !25}
!slang.llvm.version = !{!26}

!0 = distinct !DICompileUnit(language: DW_LANG_GOOGLE_RenderScript, file: !1, producer: "Android clang version 3.8.256229  (based on LLVM 3.8.256229)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, globals: !2)
!1 = !DIFile(filename: "test_slang_version_info.rs", directory: ".")
!2 = !{}
!3 = !{!4}
!4 = distinct !DISubprogram(name: "swizzle", scope: !5, file: !5, line: 5, type: !6, isLocal: false, isDefinition: true, scopeLine: 6, isOptimized: false, unit: !0, variables: !2)
!5 = !DIFile(filename: "test_slang_version_info.rs", directory: ".")
!6 = !DISubroutineType(types: !7)
!7 = !{!8, !8}
!8 = !DIDerivedType(tag: DW_TAG_typedef, name: "uchar4", file: !9, line: 328, baseType: !10)
!9 = !DIFile(filename: "frameworks/rs/scriptc/rs_value_types.rsh", directory: ".")
!10 = !DICompositeType(tag: DW_TAG_array_type, baseType: !11, size: 32, align: 32, flags: DIFlagVector, elements: !14)
!11 = !DIDerivedType(tag: DW_TAG_typedef, name: "uchar", file: !9, line: 206, baseType: !12)
!12 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint8_t", file: !9, line: 172, baseType: !13)
!13 = !DIBasicType(name: "unsigned char", size: 8, align: 8, encoding: DW_ATE_unsigned_char)
!14 = !{!15}
!15 = !DISubrange(count: 4)
!16 = !{i32 2, !"Debug Info Version", i32 3}
!17 = !{i32 1, !"wchar_size", i32 4}
!18 = !{i32 1, !"min_enum_size", i32 4}
!19 = !{!"Android clang version 3.8.256229  (based on LLVM 3.8.256229)"}
!20 = !{!"version", !"1"}
!21 = !{!"java_package_name", !"com.android.libbcc.test_version_info"}
!22 = !{!"root"}
!23 = !{!"swizzle"}
!24 = !{!"0"}
!25 = !{!"35"}
!26 = !{!"REPLACE_LLVM_VERSION_STRING_HERE"}
!27 = !DILocalVariable(name: "in", arg: 1, scope: !4, file: !5, line: 5, type: !8)
!28 = !DIExpression()
!29 = !DILocation(line: 5, scope: !4)
!30 = !DILocalVariable(name: "result", scope: !4, file: !5, line: 7, type: !8)
!31 = !DILocation(line: 7, scope: !4)
!32 = !DILocation(line: 8, scope: !4)
