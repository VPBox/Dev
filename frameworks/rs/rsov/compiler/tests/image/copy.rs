#pragma version(1)
#pragma rs java_package_name(rs2spirv)
#pragma rs_fp_relaxed

uchar4 __attribute__((kernel)) copy(uchar4 v_in) {
    return v_in;
}
