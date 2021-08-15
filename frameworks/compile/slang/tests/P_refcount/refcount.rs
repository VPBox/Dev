#pragma version(1)
#pragma rs java_package_name(foo)

rs_allocation globalAlloc;
rs_allocation globalAlloc2;

struct hasMatrix {
    rs_matrix3x3 m;
} ghm;

static void foo() {

    rs_allocation allocUninit;
    rs_allocation allocArr[10];
    allocUninit = globalAlloc;
    for (int i = 0; i < 10; i++) {
        allocArr[i] = globalAlloc;
    }

    return;
}

void singleStmt() {
    struct hasMatrix h = ghm;
    ghm = h;
    globalAlloc = globalAlloc2;
}

int root(void) {
    foo();
    return 10;
}

