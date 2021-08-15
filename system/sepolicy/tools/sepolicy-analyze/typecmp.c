#include <getopt.h>
#include <sepol/policydb/expand.h>

#include "typecmp.h"

void typecmp_usage() {
    fprintf(stderr, "\ttypecmp [-d|--diff] [-e|--equiv]\n");
}

static int insert_type_rule(avtab_key_t * k, avtab_datum_t * d,
                            struct avtab_node *type_rules)
{
    struct avtab_node *p, *c, *n;

    for (p = type_rules, c = type_rules->next; c; p = c, c = c->next) {
        /*
         * Find the insertion point, keeping the list
         * ordered by source type, then target type, then
         * target class.
         */
        if (k->source_type < c->key.source_type)
            break;
        if (k->source_type == c->key.source_type &&
            k->target_type < c->key.target_type)
            break;
        if (k->source_type == c->key.source_type &&
            k->target_type == c->key.target_type &&
            k->target_class <= c->key.target_class)
            break;
    }

    if (c &&
        k->source_type == c->key.source_type &&
        k->target_type == c->key.target_type &&
        k->target_class == c->key.target_class) {
        c->datum.data |= d->data;
        return 0;
    }

    /* Insert the rule */
    n = malloc(sizeof(struct avtab_node));
    if (!n) {
        fprintf(stderr, "out of memory\n");
        exit(1);
    }

    n->key = *k;
    n->datum = *d;
    n->next = p->next;
    p->next = n;
    return 0;
}

static int create_type_rules_helper(avtab_key_t * k, avtab_datum_t * d,
                                    void *args)
{
    struct avtab_node *type_rules = args;
    avtab_key_t key;

    /*
     * Insert the rule into the list for
     * the source type.  The source type value
     * is cleared as we want to compare against other type
     * rules with different source types.
     */
    key = *k;
    key.source_type = 0;
    if (k->source_type == k->target_type) {
        /* Clear target type as well; this is a self rule. */
        key.target_type = 0;
    }
    if (insert_type_rule(&key, d, &type_rules[k->source_type - 1]))
        return -1;

    if (k->source_type == k->target_type)
        return 0;

    /*
     * If the target type differs, then we also
     * insert the rule into the list for the target
     * type.  We clear the target type value so that
     * we can compare against other type rules with
     * different target types.
     */
    key = *k;
    key.target_type = 0;
    if (insert_type_rule(&key, d, &type_rules[k->target_type - 1]))
        return -1;

    return 0;
}

static int create_type_rules(avtab_key_t * k, avtab_datum_t * d, void *args)
{
    if (k->specified & AVTAB_ALLOWED)
        return create_type_rules_helper(k, d, args);
    return 0;
}

static int create_type_rules_cond(avtab_key_t * k, avtab_datum_t * d,
                                  void *args)
{
    if ((k->specified & (AVTAB_ALLOWED|AVTAB_ENABLED)) ==
        (AVTAB_ALLOWED|AVTAB_ENABLED))
        return create_type_rules_helper(k, d, args);
    return 0;
}

static void free_type_rules(struct avtab_node *l)
{
    struct avtab_node *tmp;

    while (l) {
        tmp = l;
        l = l->next;
        free(tmp);
    }
}

static int find_match(policydb_t *policydb, struct avtab_node *l1,
                      int idx1, struct avtab_node *l2, int idx2)
{
    struct avtab_node *c;
    uint32_t perms1, perms2;

    for (c = l2; c; c = c->next) {
        if (l1->key.source_type < c->key.source_type)
            break;
        if (l1->key.source_type == c->key.source_type &&
            l1->key.target_type < c->key.target_type)
            break;
        if (l1->key.source_type == c->key.source_type &&
            l1->key.target_type == c->key.target_type &&
            l1->key.target_class <= c->key.target_class)
            break;
    }

    if (c &&
        l1->key.source_type == c->key.source_type &&
        l1->key.target_type == c->key.target_type &&
        l1->key.target_class == c->key.target_class) {
        perms1 = l1->datum.data & ~c->datum.data;
        perms2 = c->datum.data & ~l1->datum.data;
        if (perms1 || perms2) {
            if (perms1)
                display_allow(policydb, &l1->key, idx1, perms1);
            if (perms2)
                display_allow(policydb, &c->key, idx2, perms2);
            printf("\n");
            return 1;
        }
    }

    return 0;
}

static int analyze_types(policydb_t * policydb, char diff, char equiv)
{
    avtab_t exp_avtab, exp_cond_avtab;
    struct avtab_node *type_rules, *l1, *l2;
    struct type_datum *type;
    size_t i, j;

    /*
     * Create a list of access vector rules for each type
     * from the access vector table.
     */
    type_rules = malloc(sizeof(struct avtab_node) * policydb->p_types.nprim);
    if (!type_rules) {
        fprintf(stderr, "out of memory\n");
        exit(1);
    }
    memset(type_rules, 0, sizeof(struct avtab_node) * policydb->p_types.nprim);

    if (avtab_init(&exp_avtab) || avtab_init(&exp_cond_avtab)) {
        fputs("out of memory\n", stderr);
        free(type_rules);
        return -1;
    }

    if (expand_avtab(policydb, &policydb->te_avtab, &exp_avtab)) {
        fputs("out of memory\n", stderr);
        avtab_destroy(&exp_avtab);
        free(type_rules);
        return -1;
    }

    if (expand_avtab(policydb, &policydb->te_cond_avtab, &exp_cond_avtab)) {
        fputs("out of memory\n", stderr);
        avtab_destroy(&exp_avtab); /*  */
        free(type_rules);
        return -1;
    }

    if (avtab_map(&exp_avtab, create_type_rules, type_rules)) {
        free(type_rules);
        exit(1);
    }

    if (avtab_map(&exp_cond_avtab, create_type_rules_cond, type_rules)) {
        free(type_rules);
        exit(1);
    }

    avtab_destroy(&exp_avtab);
    avtab_destroy(&exp_cond_avtab);

    /*
     * Compare the type lists and identify similar types.
     */
    for (i = 0; i < policydb->p_types.nprim - 1; i++) {
        if (!type_rules[i].next)
            continue;
        type = policydb->type_val_to_struct[i];
        if (type->flavor) {
            free_type_rules(type_rules[i].next);
            type_rules[i].next = NULL;
            continue;
        }
        for (j = i + 1; j < policydb->p_types.nprim; j++) {
            type = policydb->type_val_to_struct[j];
            if (type->flavor) {
                free_type_rules(type_rules[j].next);
                type_rules[j].next = NULL;
                continue;
            }
            for (l1 = type_rules[i].next, l2 = type_rules[j].next;
                 l1 && l2; l1 = l1->next, l2 = l2->next) {
                if (l1->key.source_type != l2->key.source_type)
                    break;
                if (l1->key.target_type != l2->key.target_type)
                    break;
                if (l1->key.target_class != l2->key.target_class
                    || l1->datum.data != l2->datum.data)
                    break;
            }
            if (l1 || l2) {
                if (diff) {
                    printf
                        ("Types %s and %s differ, starting with:\n",
                         policydb->p_type_val_to_name[i],
                         policydb->p_type_val_to_name[j]);

                    if (l1 && l2) {
                        if (find_match(policydb, l1, i, l2, j))
                            continue;
                        if (find_match(policydb, l2, j, l1, i))
                            continue;
                    }
                    if (l1)
                        display_allow(policydb, &l1->key, i, l1->datum.data);
                    if (l2)
                        display_allow(policydb, &l2->key, j, l2->datum.data);
                    printf("\n");
                }
                continue;
            }
            free_type_rules(type_rules[j].next);
            type_rules[j].next = NULL;
            if (equiv) {
                printf("Types %s and %s are equivalent.\n",
                       policydb->p_type_val_to_name[i],
                       policydb->p_type_val_to_name[j]);
            }
        }
        free_type_rules(type_rules[i].next);
        type_rules[i].next = NULL;
    }

    free(type_rules);
    return 0;
}

int typecmp_func (int argc, char **argv, policydb_t *policydb) {
    char ch, diff = 0, equiv = 0;

    struct option typecmp_options[] = {
        {"diff", no_argument, NULL, 'd'},
        {"equiv", no_argument, NULL, 'e'},
        {NULL, 0, NULL, 0}
    };

    while ((ch = getopt_long(argc, argv, "de", typecmp_options, NULL)) != -1) {
        switch (ch) {
        case 'd':
            diff = 1;
            break;
        case 'e':
            equiv = 1;
            break;
        default:
            USAGE_ERROR = true;
            return -1;
        }
    }

    if (!(diff || equiv)) {
        USAGE_ERROR = true;
        return -1;
    }
    return analyze_types(policydb, diff, equiv);
}
