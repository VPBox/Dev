#!/usr/bin/perl -w

# Generate trivial test cases to exercise input types.

use strict;

my @basicTypes = ("half", "float", "double",
                  "char", "short", "int", "long",
                  "uchar", "ushort", "uint", "ulong",
                  "bool",
                  "rs_matrix2x2", "rs_matrix3x3", "rs_matrix4x4",
                  "MyStruct");

my @specialParameters = ("context", "x", "y", "z");
my $specialParameterPowerSetCardinality = 2 ** (1 + $#specialParameters);

# 1 signifies non-vector
# 3 is not supported for exported types
my @vecLengths = (1, 2, 4);

print "// -Wall -Werror\n";
print "#pragma version(1)\n";
print "#pragma rs java_package_name(input)\n\n";
print "// This test case was created by $0.\n";
print "// It exercises various legal combinations of inputs and special parameters,\n";
print "//  so that we can ensure\n";
print "// (a) We do not choke when compiling them\n";
print "// (b) We reflect them correctly\n\n";
print "// One example struct type\n";
print "typedef struct MyStruct { float f; double d; } MyStruct;\n\n";
print "// Trivial combiner shared by all test cases\n";
print "static void combiner(int *accum, const int *other) { }\n";

foreach my $basicType (@basicTypes) {
  foreach my $vecLen (@vecLengths) {

    # There are no bool vectors or struct vectors
    next if ($vecLen > 1) && (($basicType eq "bool") || ($basicType eq "MyStruct"));

    # There are no matrix or object vectors
    next if ($vecLen > 1) && (substr($basicType, 0, 3) eq "rs_");

    my $eltName = $basicType;
    $eltName .= $vecLen if ($vecLen > 1);

    for (my $specials = 0; $specials < $specialParameterPowerSetCardinality; ++$specials) {
      my $reduceName = "my_${eltName}_${specials}";
      my $accumName = "${reduceName}_accum";
      print "\n";
      print "#pragma rs reduce(${reduceName}) accumulator(${accumName}) combiner(combiner)\n";
      print "static void ${accumName}(int *accum, ${eltName} in";
      for (my $special = 0; $special <= $#specialParameters; ++$special) {
        if ($specials & 2**$special) {
          print ", " . ($special ? "uint" : "rs_kernel_context") . " ${specialParameters[$special]}";
        }
      }
      print ") { }\n";
    }
  }
}
