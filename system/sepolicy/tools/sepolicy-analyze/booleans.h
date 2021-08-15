#ifndef BOOLEANS_H
#define BOOLEANS_H

#include <sepol/policydb/policydb.h>

#include "utils.h"

void booleans_usage(void);
int booleans_func(int argc, char **argv, policydb_t *policydb);

#endif /* BOOLEANS_H */
