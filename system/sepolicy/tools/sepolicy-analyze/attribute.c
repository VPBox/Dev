#include <getopt.h>

#include "attribute.h"

void attribute_usage() {
    fprintf(stderr, "\tattribute [-l|--list] [-r|--reverse] <name>\n");
}

static void retrieve_mapping(policydb_t *policydb, struct type_datum *dat, char *name, int reverse) {
    struct ebitmap_node *n;
    unsigned int bit;

    if (reverse) {
        ebitmap_for_each_bit(&policydb->type_attr_map[dat->s.value - 1], n, bit) {
            if (!ebitmap_node_get_bit(n, bit))
                continue;
            if (!strcmp(policydb->p_type_val_to_name[bit], name))
                continue;
            printf("%s\n", policydb->p_type_val_to_name[bit]);
        }
    } else {
        ebitmap_for_each_bit(&policydb->attr_type_map[dat->s.value - 1], n, bit) {
            if (!ebitmap_node_get_bit(n, bit))
                continue;
            printf("%s\n", policydb->p_type_val_to_name[bit]);
        }
    }
}

static int list_attribute(policydb_t *policydb, char *name, int reverse)
{
    struct type_datum *dat;

    dat = hashtab_search(policydb->p_types.table, name);
    if (!dat) {
        fprintf(stderr, "%s is not defined in this policy.\n", name);
        return -1;
    }

    if (reverse) {
        if (dat->flavor != TYPE_TYPE) {
            fprintf(stderr, "%s is an attribute not a type in this policy.\n", name);
            return -1;
        }
    } else {
        if (dat->flavor != TYPE_ATTRIB) {
            fprintf(stderr, "%s is a type not an attribute in this policy.\n", name);
            return -1;
        }
    }
    retrieve_mapping(policydb, dat, name, reverse);

    return 0;
}

static int print_attr(__attribute__ ((unused)) hashtab_key_t k,
                      hashtab_datum_t d, void *args) {
    struct type_datum *dat = (struct type_datum *)d;
    policydb_t *pdb = (policydb_t *)args;
    if (!dat) {
        fprintf(stderr, "type encountered without datum!\n");
        return -1;
    }
    if (dat->flavor == TYPE_ATTRIB) {
        printf("%s\n", pdb->p_type_val_to_name[dat->s.value - 1]);
    }
    return 0;
}

static int list_all_attributes(policydb_t *policydb) {
    return hashtab_map(policydb->p_types.table, print_attr, policydb);
}

int attribute_func (int argc, char **argv, policydb_t *policydb) {
    int rc = -1;
    int list = 0;
    int reverse = 0;
    char ch;

    struct option attribute_options[] = {
        {"list", no_argument, NULL, 'l'},
        {"reverse", no_argument, NULL, 'r'},
        {NULL, 0, NULL, 0}
    };

    while ((ch = getopt_long(argc, argv, "lr", attribute_options, NULL)) != -1) {
        switch (ch) {
        case 'l':
            list = 1;
            break;
        case 'r':
            reverse = 1;
            break;
        default:
            USAGE_ERROR = true;
            goto out;
        }
    }

    if ((argc != 2 && !(reverse && argc == 3)) || (list && reverse)) {
        USAGE_ERROR = true;
        goto out;
    }
    if (list)
        rc = list_all_attributes(policydb);
    else
        rc = list_attribute(policydb, argv[optind], reverse);
 out:
    return rc;
}
