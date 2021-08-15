#pragma version(1)
#pragma rs java_package_name(foo)

typedef struct user_t {
    int *v;
} user;

void root(int *output, const user * usr) {
   *output = *usr->v;
}
