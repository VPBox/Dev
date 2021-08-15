#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "dups.h"
#include "neverallow.h"
#include "perm.h"
#include "typecmp.h"
#include "booleans.h"
#include "attribute.h"
#include "utils.h"

#define NUM_COMPONENTS (int) (sizeof(analyze_components)/sizeof(analyze_components[0]))

#define COMP(x) { #x, sizeof(#x) - 1, x ##_usage, x ##_func }
static struct {
    const char *key;
    size_t keylen;
    void (*usage) (void);
    int (*func) (int argc, char **argv, policydb_t *policydb);
} analyze_components[] = {
    COMP(dups),
    COMP(neverallow),
    COMP(permissive),
    COMP(typecmp),
    COMP(booleans),
    COMP(attribute)
};

void usage(char *arg0)
{
    int i;

    fprintf(stderr, "%s must be called on a policy file with a component and the appropriate arguments specified\n", arg0);
    fprintf(stderr, "%s <policy-file>:\n", arg0);
    for(i = 0; i < NUM_COMPONENTS; i++) {
        analyze_components[i].usage();
    }
    exit(1);
}

int main(int argc, char **argv)
{
    char *policy;
    struct policy_file pf;
    policydb_t policydb;
    int rc;
    int i;

    if (argc < 3)
        usage(argv[0]);
    policy = argv[1];
    if(load_policy(policy, &policydb, &pf))
        exit(1);
    for(i = 0; i < NUM_COMPONENTS; i++) {
        if (!strcmp(analyze_components[i].key, argv[2])) {
            rc = analyze_components[i].func(argc - 2, argv + 2, &policydb);
            if (rc && USAGE_ERROR) {
                usage(argv[0]); }
            policydb_destroy(&policydb);
            return rc;
        }
    }
    usage(argv[0]);
    exit(0);
}
