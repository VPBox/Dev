#ifndef TYPECMP_H
#define TYPECMP_H

#include <sepol/policydb/policydb.h>

#include "utils.h"

void typecmp_usage(void);
int typecmp_func(int argc, char **argv, policydb_t *policydb);

#endif /* TYPECMP_H */
