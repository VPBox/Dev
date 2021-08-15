#pragma version(1)
#pragma rs java_package_name(foo)

struct hasMatrix {
    int j;
    rs_matrix3x3 m;
    int i;
} ghm;

extern foo(struct hasMatrix*);

void singleStmt() {
    struct hasMatrix h = ghm;
}

struct new_matrix {
    float f[2];
};

struct new_struct {
    struct new_matrix m;
    rs_matrix3x3 t;
    int i;
} newm;

void newstmt() {
    struct new_struct h = {1.f, 2.f, 3};
    struct new_struct h_uninit;
    struct new_struct empty_init = {};
}

