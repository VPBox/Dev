#include "perm.h"

void permissive_usage() {
    fprintf(stderr, "\tpermissive\n");
}

static int list_permissive(policydb_t * policydb)
{
    struct ebitmap_node *n;
    unsigned int bit;

    /*
     * iterate over all domains and check if domain is in permissive
     */
    ebitmap_for_each_bit(&policydb->permissive_map, n, bit)
    {
        if (ebitmap_node_get_bit(n, bit)) {
            printf("%s\n", policydb->p_type_val_to_name[bit -1]);
        }
    }
    return 0;
}

int permissive_func (int argc, __attribute__ ((unused)) char **argv, policydb_t *policydb) {
    if (argc != 1) {
        USAGE_ERROR = true;
        return -1;
    }
    return list_permissive(policydb);
}
