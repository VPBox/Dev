#ifndef DUPS_H
#define DUPS_H

#include <sepol/policydb/policydb.h>

#include "utils.h"

void dups_usage(void);
int dups_func(int argc, char **argv, policydb_t *policydb);

#endif /* DUPS_H */
