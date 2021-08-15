// -I .
#pragma version(1)
#pragma rs java_package_name(foo)

int __attribute__((kernel)) foo() {
    return my_extern_const_in_header;
}
