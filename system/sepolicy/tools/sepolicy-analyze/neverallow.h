#ifndef NEVERALLOW_H
#define NEVERALLOW_H

#include <sepol/policydb/policydb.h>

#include "utils.h"

void neverallow_usage(void);
int neverallow_func(int argc, char **argv, policydb_t *policydb);

#endif /* NEVERALLOW_H */
