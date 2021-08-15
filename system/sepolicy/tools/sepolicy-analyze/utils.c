#include <fcntl.h>
#include <sepol/policydb/policydb.h>
#include <sepol/policydb/util.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "utils.h"

bool USAGE_ERROR = false;

void display_allow(policydb_t *policydb, avtab_key_t *key, int idx, uint32_t perms)
{
    printf("    allow %s %s:%s { %s };\n",
           policydb->p_type_val_to_name[key->source_type
                                        ? key->source_type - 1 : idx],
           key->target_type == key->source_type ? "self" :
           policydb->p_type_val_to_name[key->target_type
                                        ? key->target_type - 1 : idx],
           policydb->p_class_val_to_name[key->target_class - 1],
           sepol_av_to_string
           (policydb, key->target_class, perms));
}

int load_policy(char *filename, policydb_t * policydb, struct policy_file *pf)
{
    int fd;
    struct stat sb;
    void *map;
    int ret;

    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "Can't open '%s':  %s\n", filename, strerror(errno));
        return 1;
    }
    if (fstat(fd, &sb) < 0) {
        fprintf(stderr, "Can't stat '%s':  %s\n", filename, strerror(errno));
        close(fd);
        return 1;
    }
    map = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (map == MAP_FAILED) {
        fprintf(stderr, "Can't mmap '%s':  %s\n", filename, strerror(errno));
        close(fd);
        return 1;
    }

    policy_file_init(pf);
    pf->type = PF_USE_MEMORY;
    pf->data = map;
    pf->len = sb.st_size;
    if (policydb_init(policydb)) {
        fprintf(stderr, "Could not initialize policydb!\n");
        close(fd);
        munmap(map, sb.st_size);
        return 1;
    }
    ret = policydb_read(policydb, pf, 0);
    if (ret) {
        fprintf(stderr, "error(s) encountered while parsing configuration\n");
        close(fd);
        munmap(map, sb.st_size);
        return 1;
    }

    return 0;
}
