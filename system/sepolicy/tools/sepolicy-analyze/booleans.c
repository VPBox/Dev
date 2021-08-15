#include "booleans.h"

void booleans_usage() {
    fprintf(stderr, "\tbooleans\n");
}

static int list_booleans(hashtab_key_t k,
                         __attribute__ ((unused)) hashtab_datum_t d,
                         __attribute__ ((unused)) void *args)
{
    const char *name = k;
    printf("%s\n", name);
    return 0;
}

int booleans_func (int argc, __attribute__ ((unused)) char **argv, policydb_t *policydb) {
    if (argc != 1) {
        USAGE_ERROR = true;
        return -1;
    }
    return hashtab_map(policydb->p_bools.table, list_booleans, NULL);
}
