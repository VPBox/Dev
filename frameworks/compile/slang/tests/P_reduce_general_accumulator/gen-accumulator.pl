#!/usr/bin/perl -w

# Generate trivial test cases to exercise accumulator types.

use strict;

my @basicTypesVectorable = ("half", "float", "double",
                            "char", "short", "int", "long",
                            "uchar", "ushort", "uint", "ulong");
my @basicTypesUnvectorable = ("bool",
                              "rs_matrix2x2", "rs_matrix3x3", "rs_matrix4x4",
                              "rs_for_each_strategy_t", "rs_script_call_t", "rs_time_t", "rs_tm",
                              "MyEnum",
                              "MyPtrArray", "MyPtrFn", "MyPtrIncomplete", "MyPtrShort", "MyPtrVoid",
                              "MyStruct", "MyUnion",
                              "MyBlob");
my @basicTypes = (@basicTypesVectorable, @basicTypesUnvectorable);

# 1 signifies non-vector
# 3 is not supported for exported types
my @vecLengths = (1, 2, 4);

print "// -target-api 0 -Wall -Werror\n";
print "#pragma version(1)\n";
print "#pragma rs java_package_name(accumulator)\n\n";
print "// This test case was created by $0.\n";
print "// It exercises various legal accumulator types, so that we can ensure\n";
print "// (a) We do not choke when compiling them\n";
print "// (b) They do not inhibit reflection\n";
print "\n// One example enum type\n";
print "typedef enum { E1, E2, E3 } MyEnum;\n";
print "\n// Example pointer types\n";
print "typedef float (*MyPtrArray)[3][11];\n";
print "typedef int (*MyPtrFn)(double);\n";
print "typedef struct Incomplete *MyPtrIncomplete;\n";
print "typedef volatile short *MyPtrShort;\n";
print "typedef const void *MyPtrVoid;\n";
print "\n// One example struct type\n";
print "typedef struct { float f; double d; char *c; } MyStruct;\n";
print "\n// One example union type\n";
print "typedef union { int i; long l; int a[5]; } MyUnion;\n";
print "\n// One example of a more-complicated type\n";
print "typedef struct { MyEnum e1, e2; MyPtrIncomplete p; MyStruct s; MyUnion u; } MyBlob;\n";

foreach my $basicType (@basicTypes) {
  for (my $isArray = 0; $isArray <= 1; ++$isArray) {
    foreach my $vecLen (@vecLengths) {

      next if ($vecLen > 1) && !grep(/^${basicType}$/, @basicTypesVectorable);

      my $eltName = $basicType;
      $eltName .= $vecLen if ($vecLen > 1);
      my $resultName = ($isArray ? "array_${eltName}" : $eltName);
      my $reduceName = "my_${resultName}";
      my $accumName = "${reduceName}_accum";
      my $combName = "${reduceName}_comb";
      my $outName = "${reduceName}_out";
      print "\n";
      print "#pragma rs reduce(${reduceName}) accumulator(${accumName}) combiner(${combName}) outconverter(${outName})\n";
      print "typedef ${eltName} ${resultName}[7];\n" if ($isArray);
      print "static void ${accumName}(${resultName} *accum, int val) { }\n";
      print "static void ${combName}(${resultName} *accum, const ${resultName} *other) { }\n";
      print "static void ${outName}(int *out, const ${resultName} *accum) { }\n";
    }
  }
}
