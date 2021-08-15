#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <sepol/policydb/policydb.h>

#include "utils.h"

void attribute_usage(void);
int attribute_func(int argc, char **argv, policydb_t *policydb);

#endif /* ATTRIBUTE_H */
