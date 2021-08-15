#pragma version(1)
#pragma rs java_package_name(foo)

// "Divergent" = reflected code must have a runtime check for 32-bit
// versus 64-bit target.

// non-divergent
int intVar;

// non-divergent
int intArray[10];

// non-divergent
rs_matrix2x2 matVar;

// non-divergent
rs_matrix2x2 matArray[10];

// divergent
rs_allocation allocVar;

// divergent
rs_allocation allocArray[10];

struct NonDivergent {
  int i;
  int j;
};

struct NonDivergent ndVar;

struct NonDivergent ndArray[10];

// 32-bit: 12 bytes; 64-bit: 48 bytes
struct Divergent {
  int i;
  rs_allocation a;
  int j;
};

struct Divergent dVar;

struct Divergent dArray[10];

// 32-bit: 20 bytes; 64-bit: 64 bytes
struct DivergentNest {
  int x;
  struct Divergent d;
  int y;
};

#if 0

// TODO: Add these variables back once http://b/65210157 is fixed

struct DivergentNest dnVar;

struct DivergentNest dnArray[10];

#endif

void intFe(const int *in, int *out, const int *data) { }

void matFe(const int *in, int *out, const rs_matrix2x2 *data) { }

void allocFe(const int *in, int *out, const rs_allocation *data) { }

void ndFe(const int *in, int *out, const struct NonDivergent *data) { }

void dFe(const int *in, int *out, const struct Divergent *data) { }

void dnFe(const int *in, int *out, const struct DivergentNest *data) { }

// for arguments, should get a helper struct that looks like struct NonDivergent
void ndInv(int i, int j) { }

// for arguments, should get a helper struct that looks like struct Divergent
void dInv(int i, rs_allocation a, int j) { (void)a; }

// for arguments, should get a helper struct that looks like struct DivergentNest
void dnInv(int x, struct Divergent d, int y) { (void)d; }
