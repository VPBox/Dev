// -target-api=22
#pragma version(1)
#pragma rs java_package_name(foo)

__fp16 RS_KERNEL foo(__fp16 a) {
  return (a+1);
}
