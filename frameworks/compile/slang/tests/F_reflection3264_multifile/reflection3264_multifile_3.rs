#pragma version(1)
#pragma rs java_package_name(foo)

// we stop compiling after the first file with an error;
// reflection3264_multifile_2.rs has errors, so we never get here, and
// never report errors for this file's 'v' and 'w'.

size_t v, w;
