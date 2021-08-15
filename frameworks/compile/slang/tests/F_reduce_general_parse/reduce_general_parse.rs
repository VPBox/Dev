// -Wall -Werror
#pragma version(1)
#pragma rs java_package_name(all)

#pragma rs reduce
#pragma rs reduce(
#pragma rs reduce (
#pragma rs reduce(*
#pragma rs reduce(foo
#pragma rs reduce   (foo
#pragma rs reduce(foo  )
#pragma rs reduce(foo) initializer(
#pragma rs reduce(foo) initializer(init)
#pragma rs reduce(foo) initializer(init) accumulator(accum)
#pragma rs reduce(foo) initializer(init2) accumulator(accum2)
#pragma rs reduce(bar) accumulator(baccum) initializer(binit)
#pragma rs reduce(baz) accumulator(baccum) initializer(binit) accumulator(baccum)
#pragma rs reduce(baz) accumulator(baccum) initializer(binit) accumulator(baccum2)
#pragma rs reduce(goo) something(wrong)
#pragma rs reduce(goo) initializer(gi) accumulator(ga) (

static void init(int *accum) { }
static void accum(int *accum, int val) { }

static void binit(int *accum) { }
static void baccum(int *accum, int val) { }
