#include <stdbool.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "dups.h"

void dups_usage() {
    fprintf(stderr, "\tdups\n");
}

static int find_dups_helper(avtab_key_t * k, avtab_datum_t * d,
                            void *args)
{
    policydb_t *policydb = args;
    ebitmap_t *sattr, *tattr;
    ebitmap_node_t *snode, *tnode;
    unsigned int i, j;
    avtab_key_t avkey;
    avtab_ptr_t node;
    struct type_datum *stype, *ttype, *stype2, *ttype2;
    bool attrib1, attrib2;

    if (!(k->specified & AVTAB_ALLOWED))
        return 0;

    if (k->source_type == k->target_type)
        return 0; /* self rule */

    avkey.target_class = k->target_class;
    avkey.specified = k->specified;

    sattr = &policydb->type_attr_map[k->source_type - 1];
    tattr = &policydb->type_attr_map[k->target_type - 1];
    stype = policydb->type_val_to_struct[k->source_type - 1];
    ttype = policydb->type_val_to_struct[k->target_type - 1];
    attrib1 = stype->flavor || ttype->flavor;
    ebitmap_for_each_bit(sattr, snode, i) {
        if (!ebitmap_node_get_bit(snode, i))
            continue;
        ebitmap_for_each_bit(tattr, tnode, j) {
            if (!ebitmap_node_get_bit(tnode, j))
                continue;
            avkey.source_type = i + 1;
            avkey.target_type = j + 1;
            if (avkey.source_type == k->source_type &&
                avkey.target_type == k->target_type)
                continue;
            if (avkey.source_type == avkey.target_type)
                continue; /* self rule */
            stype2 = policydb->type_val_to_struct[avkey.source_type - 1];
            ttype2 = policydb->type_val_to_struct[avkey.target_type - 1];
            attrib2 = stype2->flavor || ttype2->flavor;
            if (attrib1 && attrib2)
                continue; /* overlapping attribute-based rules */
            for (node = avtab_search_node(&policydb->te_avtab, &avkey);
                 node != NULL;
                 node = avtab_search_node_next(node, avkey.specified)) {
                uint32_t perms = node->datum.data & d->data;
                if ((attrib1 && perms == node->datum.data) ||
                    (attrib2 && perms == d->data)) {
                    /*
                     * The attribute-based rule is a superset of the
                     * non-attribute-based rule.  This is a dup.
                     */
                    printf("Duplicate allow rule found:\n");
                    display_allow(policydb, k, i, d->data);
                    display_allow(policydb, &node->key, i, node->datum.data);
                    printf("\n");
                }
            }
        }
    }

    return 0;
}

static int find_dups(policydb_t * policydb)
{
    if (avtab_map(&policydb->te_avtab, find_dups_helper, policydb))
        return -1;
    return 0;
}

int dups_func (int argc, __attribute__ ((unused)) char **argv, policydb_t *policydb) {
    if (argc != 1) {
        USAGE_ERROR = true;
        return -1;
    }
    return find_dups(policydb);
}
