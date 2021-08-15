#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stdint.h>
#include <sepol/policydb/avtab.h>
#include <sepol/policydb/policydb.h>


extern bool USAGE_ERROR;

void display_allow(policydb_t *policydb, avtab_key_t *key, int idx, uint32_t perms);

int load_policy(char *filename, policydb_t * policydb, struct policy_file *pf);

#endif /* UTILS_H */
