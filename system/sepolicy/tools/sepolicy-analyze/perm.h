#ifndef PERM_H
#define PERM_H

#include <sepol/policydb/policydb.h>

#include "utils.h"

void permissive_usage(void);
int permissive_func(int argc, char **argv, policydb_t *policydb);

#endif /* PERM_H */
