; This test assembles this file to bitcode with all supported target
; API versions, then checks that the bitcode file was generated and
; has the right magic number.

; RUN: %llvm-rs-as -target-api 11 %s -o %t11
; RUN: xxd -ps -l 4 %t11 | FileCheck %s
; RUN: %llvm-rs-as -target-api 12 %s -o %t12
; RUN: xxd -ps -l 4 %t12 | FileCheck %s
; RUN: %llvm-rs-as -target-api 13 %s -o %t13
; RUN: xxd -ps -l 4 %t13 | FileCheck %s
; RUN: %llvm-rs-as -target-api 14 %s -o %t14
; RUN: xxd -ps -l 4 %t14 | FileCheck %s
; RUN: %llvm-rs-as -target-api 15 %s -o %t15
; RUN: xxd -ps -l 4 %t15 | FileCheck %s
; RUN: %llvm-rs-as -target-api 16 %s -o %t16
; RUN: xxd -ps -l 4 %t16 | FileCheck %s
; RUN: %llvm-rs-as -target-api 17 %s -o %t17
; RUN: xxd -ps -l 4 %t17 | FileCheck %s
; RUN: %llvm-rs-as -target-api 18 %s -o %t18
; RUN: xxd -ps -l 4 %t18 | FileCheck %s
; RUN: %llvm-rs-as -target-api 19 %s -o %t19
; RUN: xxd -ps -l 4 %t19 | FileCheck %s
; RUN: %llvm-rs-as -target-api 20 %s -o %t20
; RUN: xxd -ps -l 4 %t20 | FileCheck %s
; RUN: %llvm-rs-as -target-api 21 %s -o %t21
; RUN: xxd -ps -l 4 %t21 | FileCheck %s
; RUN: %llvm-rs-as -target-api 22 %s -o %t22
; RUN: xxd -ps -l 4 %t22 | FileCheck %s
; RUN: %llvm-rs-as -target-api 23 %s -o %t23
; RUN: xxd -ps -l 4 %t23 | FileCheck %s

; RUN: %llvm-rs-as -target-api 0 %s -o %t0
; RUN: xxd -ps -l 4 %t0 | FileCheck %s

; Check for the magic number.

; CHECK: dec0170b

; ModuleID = 'kernel.bc'
target datalayout = "e-p:32:32-i64:64-v128:64:128-n32-S64"
target triple = "armv7-none-linux-gnueabi"

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}
!\23pragma = !{!3, !4}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"min_enum_size", i32 4}
!2 = !{!"clang version 3.6 "}
!3 = !{!"version", !"1"}
!4 = !{!"java_package_name", !"foo"}
