#!/usr/bin/perl -w

# Generate trivial test cases to exercise result types.

use strict;

my @basicTypes = ("half", "float", "double",
                  "char", "short", "int", "long",
                  "uchar", "ushort", "uint", "ulong",
                  "bool",
                  "MyStruct");

# 1 signifies non-vector
# 3 is not supported for exported types
my @vecLengths = (1, 2, 4);

print "// -Wall -Werror\n";
print "#pragma version(1)\n";
print "#pragma rs java_package_name(result)\n\n";
print "// This test case was created by $0.\n";
print "// It exercises all legal Java-reflectable result types, so that we can ensure\n";
print "// (a) We do not choke when compiling them\n";
print "// (b) We reflect them correctly\n\n";
print "// One example struct type\n";
print "typedef struct MyStruct { float f; double d; } MyStruct;\n";

foreach my $basicType (@basicTypes) {
  for (my $isArray = 0; $isArray <= 1; ++$isArray) {
    foreach my $vecLen (@vecLengths) {

      # There are no bool vectors or struct vectors
      next if ($vecLen > 1) && (($basicType eq "bool") || ($basicType eq "MyStruct"));

      my $eltName = $basicType;
      $eltName .= $vecLen if ($vecLen > 1);
      my $resultName = ($isArray ? "array_${eltName}" : $eltName);
      my $reduceName = "my_${resultName}";
      my $accumName = "${reduceName}_accum";
      my $combName = "${reduceName}_comb";
      print "\n";
      print "#pragma rs reduce(${reduceName}) accumulator(${accumName}) combiner(${combName})\n";
      print "typedef ${eltName} ${resultName}[7];\n" if ($isArray);
      print "static void ${accumName}(${resultName} *accum, ${eltName} val) { }\n";
      print "static void ${combName}(${resultName} *accum, const ${resultName} *other) { }\n";
    }
  }
}
