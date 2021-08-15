#pragma version(1)
#pragma rs java_package_name(foo)

struct t { int f; };

struct q { int f; };

struct b { int f; };

struct j { int f; };

struct f { int f; };

struct o { int f; };

// disagreement as to which struct types are exported

#ifdef __LP64__
struct t aa;
struct q bb;
struct b cc;
int dd;
int ee;
int ff;
#else
int aa;
int bb;
int cc;
struct j dd;
struct f ee;
struct o ff;
#endif

// disagreeement as to how many fields in an exported struct

#ifdef __LP64__
struct FieldCount {
  int a;
  float b;
};
#else
struct FieldCount {
  double c;
};
#endif

struct FieldCount gg;

// disagreement as to field name in an exported struct
// (after first mismatched name, we do not report
// any other errors for the struct)

#ifdef __LP64__
struct FieldName {
  int a;
  int b;
  int c;
  int d;
};
#else
struct FieldName {
  int a;
  int e;
  float c;
  int f;
};
#endif

struct FieldName hh;

// disagreement as to field types in an exported struct

struct FieldType {
  int a;
  size_t b;
  rs_allocation c;
  float d;
  char e[sizeof(size_t)];
};

struct FieldType ii;
