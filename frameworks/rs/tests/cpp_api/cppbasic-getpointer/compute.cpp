
#include "RenderScript.h"

#include "ScriptC_mono.h"

#include <stdlib.h>

const uint32_t DIMX = 128;
const uint32_t DIMY = 128;

int test_compute()
{
    bool failed = false;

    sp<RS> rs = new RS();
    printf("New RS %p\n", rs.get());

    // only legitimate because this is a standalone executable
    bool r = rs->init("/system/bin");
    printf("Init returned %i\n", r);

    sp<const Element> e = Element::U32(rs);
    printf("Element %p\n", e.get());

    Type::Builder tb(rs, e);
    tb.setX(DIMX);
    tb.setY(DIMY);
    sp<const Type> t = tb.create();
    printf("Type %p\n", t.get());


    sp<Allocation> a1 = Allocation::createSized(rs, e, 1000);
    printf("Allocation %p\n", a1.get());

    sp<Allocation> ain = Allocation::createTyped(rs, t, RS_ALLOCATION_USAGE_SCRIPT | RS_ALLOCATION_USAGE_SHARED);
    sp<Allocation> aout = Allocation::createTyped(rs, t, RS_ALLOCATION_USAGE_SCRIPT | RS_ALLOCATION_USAGE_SHARED);
    printf("Allocation %p %p\n", ain.get(), aout.get());

    size_t inputStride, outputStride;

    uint32_t *input = (uint32_t*)ain->getPointer(&inputStride);
    uint32_t *output = (uint32_t*)aout->getPointer(&outputStride);

    printf("Input pointer: %p\n", input);
    printf("Input stride: %zu\n", inputStride);
    printf("Output pointer: %p\n", output);
    printf("Output stride: %zu\n", outputStride);

    inputStride /= sizeof(uint32_t);
    outputStride /= sizeof(uint32_t);

    for (size_t i = 0; i < DIMY; i++) {
        for (size_t j = 0; j < DIMX; j++) {
            input[i * inputStride + j] = rand();
            output[i * inputStride + j] = 0;
        }
    }

    ain->syncAll(RS_ALLOCATION_USAGE_SHARED);
    aout->syncAll(RS_ALLOCATION_USAGE_SHARED);

    printf("Launching script\n");

    sp<ScriptC_mono> sc = new ScriptC_mono(rs);
    sc->forEach_copyAndNot(ain, aout);
    rs->finish();

    printf("Script completed\n");

    ain->syncAll(RS_ALLOCATION_USAGE_SCRIPT);
    aout->syncAll(RS_ALLOCATION_USAGE_SCRIPT);

    for (size_t i = 0; i < DIMY; i++) {
        for (size_t j = 0; j < DIMX; j++) {
            if (input[i * inputStride + j] != ~(output[i * inputStride + j])) {
                printf("Mismatch at location %zu, %zu\n", j, i);
                failed = true;
                return failed;
            }
        }
    }


    return failed;
}

int main() {
    bool failed = test_compute();

    if (failed) {
        printf("TEST FAILED!\n");
    } else {
        printf("TEST PASSED!\n");
    }

    return failed;
}
