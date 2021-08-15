// Fake rs_core.rsh header file
//
// We use -I . to pick up this header file implicitly, instead of the proper
// rs_core.rsh header file.

// Declare an uninitialized external constant, which should be ok for our
// official header files.
extern const int my_extern_const_in_header;
