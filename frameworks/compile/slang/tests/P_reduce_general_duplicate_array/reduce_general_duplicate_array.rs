// -target-api 0 -Wall
#pragma version(1)
#pragma rs java_package_name(array)

// The two kernels have anonymous result types that are equivalent.
// slang doesn't common them (i.e., each gets its own RSExportType);
// so Java reflection must guard against this to avoid creating two
// copies of the text that defines the reflected class resultArray4_int.

static void accumFn(int *accumDatum, int val) { *accumDatum += val; }

#pragma rs reduce(sumDec) accumulator(accumFn) outconverter(outFnDec)
static void outFnDec(int (*out)[4], const int *accumDatum) {
  for (int i = 0; i < 4; ++i)
    (*out)[i] = (*accumDatum)/(i+1);
}

#pragma rs reduce(sumInc) accumulator(accumFn) outconverter(outFnInc)
static void outFnInc(int (*out)[4], const int *accumDatum) {
  for (int i = 0; i < 4; ++i)
    (*out)[i] = (*accumDatum)/(4-i);
}
