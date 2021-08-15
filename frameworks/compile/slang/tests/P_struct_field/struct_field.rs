// -Wall -Werror
#pragma version(1)
#pragma rs java_package_name(struct_field)

// Ensure that we can compile a script containing an exported struct
// with multiple fields of type struct, and reflect correct Java code
// for it.

struct InnerOne {
    int x;
    int y;
    float f;
};

typedef struct InnerOne s_innerOne;

struct InnerTwo {
    int8_t z;
    struct InnerOne innerOne;
};

struct Outer {
    struct InnerOne innerOneA;
    long l;  // to induce padding
    struct InnerOne innerOneB;
    struct InnerTwo innerTwo3[3];
    struct InnerTwo innerTwo2[2];
    struct InnerOne innerOne4[4];
    s_innerOne      innerOneC;  // does a typedef confuse reflection?
};

struct Outer outer;
