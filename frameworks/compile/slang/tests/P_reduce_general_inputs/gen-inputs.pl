#!/usr/bin/perl -w

# Generate trivial test cases to exercise input types.

use strict;

# deliberately non-exhaustive
my @basicTypes = ("half", "float", # "double",
                  "char", "short", # "int", "long",
                  "uchar", "ushort", # "uint", "ulong",
                  "bool",
                  "rs_matrix2x2", # "rs_matrix3x3", "rs_matrix4x4",
                  "MyStruct");

# 1 signifies non-vector
# 3 is not supported for exported types
my @vecLengths = (1, 2, 4);

sub isVectorEligible {
  my ($type) = @_;

  # There are no bool vectors or struct vectors
  return 0 if ($type eq "bool") || ($type eq "MyStruct");

  # There are no matrix or object vectors
  return 0 if (substr($type, 0, 3) eq "rs_");

  # Else ok
  return 1;
}

print "// -Wall -Werror\n";
print "#pragma version(1)\n";
print "#pragma rs java_package_name(inputs)\n\n";
print "// This test case was created by $0.\n";
print "// It exercises various legal combinations of inputs and special parameters,\n";
print "// so that we can ensure\n";
print "// (a) We do not choke when compiling them\n";
print "// (b) We reflect them correctly\n\n";
print "// One example struct type\n";
print "typedef struct MyStruct { float f; double d; } MyStruct;\n\n";
print "// Trivial combiner shared by all test cases\n";
print "static void combiner(int *accum, const int *other) { }\n";

foreach my $basicTypeA (@basicTypes) {
  foreach my $vecLenA (@vecLengths) {
    next if ($vecLenA > 1) && !isVectorEligible($basicTypeA);

    my $eltNameA = $basicTypeA;
    $eltNameA .= $vecLenA if ($vecLenA > 1);

    foreach my $basicTypeB (@basicTypes) {
      foreach my $vecLenB (@vecLengths) {
        next if ($vecLenB > 1) && !isVectorEligible($basicTypeB);

        my $eltNameB = $basicTypeB;
        $eltNameB .= $vecLenB if ($vecLenB > 1);

        for (my $hasSpecial = 0; $hasSpecial <= 1; ++$hasSpecial) {
          my $reduceName = "my_${eltNameA}_${eltNameB}_${hasSpecial}";
          my $accumName = "${reduceName}_accum";
          print "\n";
          print "#pragma rs reduce(${reduceName}) accumulator(${accumName}) combiner(combiner)\n";
          print "static void ${accumName}(int *accum, ${eltNameA} a, ${eltNameB} b";
          print ", rs_kernel_context context" if ($hasSpecial);
          print ") { }\n";
        }
      }
    }
  }
}
