#include <ctype.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "neverallow.h"

static int debug;
static int warn;

void neverallow_usage() {
    fprintf(stderr, "\tneverallow [-w|--warn] [-d|--debug] [-n|--neverallows <neverallow-rules>] | [-f|--file <neverallow-file>]\n");
}

static int read_typeset(policydb_t *policydb, char **ptr, char *end,
                        type_set_t *typeset, uint32_t *flags)
{
    const char *keyword = "self";
    size_t keyword_size = strlen(keyword), len;
    char *p = *ptr;
    unsigned openparens = 0;
    char *start, *id;
    type_datum_t *type;
    struct ebitmap_node *n;
    unsigned int bit;
    bool negate = false;
    int rc;

    do {
        while (p < end && isspace(*p))
            p++;

        if (p == end)
            goto err;

        if (*p == '~') {
            if (debug)
                printf(" ~");
            typeset->flags = TYPE_COMP;
            p++;
            while (p < end && isspace(*p))
                p++;
            if (p == end)
                goto err;
        }

        if (*p == '{') {
            if (debug && !openparens)
                printf(" {");
            openparens++;
            p++;
            continue;
        }

        if (*p == '}') {
            if (debug && openparens == 1)
                printf(" }");
            if (openparens == 0)
                goto err;
            openparens--;
            p++;
            continue;
        }

        if (*p == '*') {
            if (debug)
                printf(" *");
            typeset->flags = TYPE_STAR;
            p++;
            continue;
        }

        if (*p == '-') {
            if (debug)
                printf(" -");
            negate = true;
            p++;
            continue;
        }

        if (*p == '#') {
            while (p < end && *p != '\n')
                p++;
            continue;
        }

        start = p;
        while (p < end && !isspace(*p) && *p != ':' && *p != ';' && *p != '{' && *p != '}' && *p != '#')
            p++;

        if (p == start)
            goto err;

        len = p - start;
        if (len == keyword_size && !strncmp(start, keyword, keyword_size)) {
            if (debug)
                printf(" self");
            *flags |= RULE_SELF;
            continue;
        }

        id = calloc(1, len + 1);
        if (!id)
            goto err;
        memcpy(id, start, len);
        if (debug)
            printf(" %s", id);
        type = hashtab_search(policydb->p_types.table, id);
        if (!type) {
            if (warn)
                fprintf(stderr, "Warning!  Type or attribute %s used in neverallow undefined in policy being checked.\n", id);
            negate = false;
            continue;
        }
        free(id);

        if (type->flavor == TYPE_ATTRIB) {
            if (negate)
                rc = ebitmap_union(&typeset->negset, &policydb->attr_type_map[type->s.value - 1]);
            else
                rc = ebitmap_union(&typeset->types, &policydb->attr_type_map[type->s.value - 1]);
        } else if (negate) {
            rc = ebitmap_set_bit(&typeset->negset, type->s.value - 1, 1);
        } else {
            rc = ebitmap_set_bit(&typeset->types, type->s.value - 1, 1);
        }

        negate = false;

        if (rc)
            goto err;

    } while (p < end && openparens);

    if (p == end)
        goto err;

    if (typeset->flags & TYPE_STAR) {
        for (bit = 0; bit < policydb->p_types.nprim; bit++) {
            if (ebitmap_get_bit(&typeset->negset, bit))
                continue;
            if (policydb->type_val_to_struct[bit] &&
                policydb->type_val_to_struct[bit]->flavor == TYPE_ATTRIB)
                continue;
            if (ebitmap_set_bit(&typeset->types, bit, 1))
                goto err;
        }
    }

    ebitmap_for_each_bit(&typeset->negset, n, bit) {
        if (!ebitmap_node_get_bit(n, bit))
            continue;
        if (ebitmap_set_bit(&typeset->types, bit, 0))
            goto err;
    }

    if (typeset->flags & TYPE_COMP) {
        for (bit = 0; bit < policydb->p_types.nprim; bit++) {
            if (policydb->type_val_to_struct[bit] &&
                policydb->type_val_to_struct[bit]->flavor == TYPE_ATTRIB)
                continue;
            if (ebitmap_get_bit(&typeset->types, bit))
                ebitmap_set_bit(&typeset->types, bit, 0);
            else {
                if (ebitmap_set_bit(&typeset->types, bit, 1))
                    goto err;
            }
        }
    }

    *ptr = p;
    return 0;
err:
    return -1;
}

static int read_classperms(policydb_t *policydb, char **ptr, char *end,
                           class_perm_node_t **perms)
{
    char *p = *ptr;
    unsigned openparens = 0;
    char *id, *start;
    class_datum_t *cls = NULL;
    perm_datum_t *perm = NULL;
    class_perm_node_t *classperms = NULL, *node = NULL;
    bool complement = false;

    while (p < end && isspace(*p))
        p++;

    if (p == end || *p != ':')
        goto err;
    p++;

    if (debug)
        printf(" :");

    do {
        while (p < end && isspace(*p))
            p++;

        if (p == end)
            goto err;

        if (*p == '{') {
            if (debug && !openparens)
                printf(" {");
            openparens++;
            p++;
            continue;
        }

        if (*p == '}') {
            if (debug && openparens == 1)
                printf(" }");
            if (openparens == 0)
                goto err;
            openparens--;
            p++;
            continue;
        }

        if (*p == '#') {
            while (p < end && *p != '\n')
                p++;
            continue;
        }

        start = p;
        while (p < end && !isspace(*p) && *p != '{' && *p != '}' && *p != ';' && *p != '#')
            p++;

        if (p == start)
            goto err;

        id = calloc(1, p - start + 1);
        if (!id)
            goto err;
        memcpy(id, start, p - start);
        if (debug)
            printf(" %s", id);
        cls = hashtab_search(policydb->p_classes.table, id);
        if (!cls) {
            if (warn)
                fprintf(stderr, "Warning!  Class %s used in neverallow undefined in policy being checked.\n", id);
            continue;
        }

        node = calloc(1, sizeof *node);
        if (!node)
            goto err;
        node->tclass = cls->s.value;
        node->next = classperms;
        classperms = node;
        free(id);
        id = NULL;
    } while (p < end && openparens);

    if (p == end)
        goto err;

    if (warn && !classperms)
        fprintf(stderr, "Warning!  Empty class set\n");

    do {
        while (p < end && isspace(*p))
            p++;

        if (p == end)
            goto err;

        if (*p == '~') {
            if (debug)
                printf(" ~");
            complement = true;
            p++;
            while (p < end && isspace(*p))
                p++;
            if (p == end)
                goto err;
        }

        if (*p == '{') {
            if (debug && !openparens)
                printf(" {");
            openparens++;
            p++;
            continue;
        }

        if (*p == '}') {
            if (debug && openparens == 1)
                printf(" }");
            if (openparens == 0)
                goto err;
            openparens--;
            p++;
            continue;
        }

        if (*p == '#') {
            while (p < end && *p != '\n')
                p++;
            continue;
        }

        start = p;
        while (p < end && !isspace(*p) && *p != '{' && *p != '}' && *p != ';' && *p != '#')
            p++;

        if (p == start)
            goto err;

        id = calloc(1, p - start + 1);
        if (!id)
            goto err;
        memcpy(id, start, p - start);
        if (debug)
            printf(" %s", id);

        if (!strcmp(id, "*")) {
            for (node = classperms; node; node = node->next)
                node->data = ~0;
            free(id);
            id = NULL;
            continue;
        }

        for (node = classperms; node; node = node->next) {
            cls = policydb->class_val_to_struct[node->tclass-1];
            perm = hashtab_search(cls->permissions.table, id);
            if (cls->comdatum && !perm)
                perm = hashtab_search(cls->comdatum->permissions.table, id);
            if (!perm) {
                if (warn)
                    fprintf(stderr, "Warning!  Permission %s used in neverallow undefined in class %s in policy being checked.\n", id, policydb->p_class_val_to_name[node->tclass-1]);
                continue;
            }
            node->data |= 1U << (perm->s.value - 1);
        }
        free(id);
        id = NULL;
    } while (p < end && openparens);

    if (p == end)
        goto err;

    if (complement) {
        for (node = classperms; node; node = node->next)
            node->data = ~node->data;
    }

    if (warn) {
        for (node = classperms; node; node = node->next)
            if (!node->data)
                fprintf(stderr, "Warning!  Empty permission set\n");
    }

    *perms = classperms;
    *ptr = p;
    return 0;
err:
    // free classperms memory
    for (node = classperms; node; ) {
      class_perm_node_t *freeptr = node;
      node = node->next;
      free(freeptr);
    }
    return -1;
}

static int check_neverallows(policydb_t *policydb, char *text, char *end)
{
    const char *keyword = "neverallow";
    size_t keyword_size = strlen(keyword), len;
    struct avrule *neverallows = NULL, *avrule;
    char *p, *start;
    int result;

    p = text;
    while (p < end) {
        while (p < end && isspace(*p))
            p++;

        if (*p == '#') {
            while (p < end && *p != '\n')
                p++;
            continue;
        }

        start = p;
        while (p < end && !isspace(*p))
            p++;

        len = p - start;
        if (len != keyword_size || strncmp(start, keyword, keyword_size))
            continue;

        if (debug)
            printf("neverallow");

        avrule = calloc(1, sizeof *avrule);
        if (!avrule)
            goto err;

        avrule->specified = AVRULE_NEVERALLOW;

        if (read_typeset(policydb, &p, end, &avrule->stypes, &avrule->flags))
            goto err;

        if (read_typeset(policydb, &p, end, &avrule->ttypes, &avrule->flags))
            goto err;

        if (read_classperms(policydb, &p, end, &avrule->perms))
            goto err;

        while (p < end && *p != ';')
            p++;

        if (p == end || *p != ';')
            goto err;

        if (debug)
            printf(";\n");

        avrule->next = neverallows;
        neverallows = avrule;
    }

    if (!neverallows)
        goto err;

    result = check_assertions(NULL, policydb, neverallows);
    avrule_list_destroy(neverallows);
    return result;
err:
    if (errno == ENOMEM) {
        fprintf(stderr, "Out of memory while parsing neverallow rules\n");
    } else
        fprintf(stderr, "Error while parsing neverallow rules\n");

    avrule_list_destroy(neverallows);
    if (avrule != neverallows)
        avrule_destroy(avrule);

    return -1;
}

static int check_neverallows_file(policydb_t *policydb, const char *filename)
{
    int fd;
    struct stat sb;
    char *text, *end;

    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "Could not open %s:  %s\n", filename, strerror(errno));
        return -1;
    }
    if (fstat(fd, &sb) < 0) {
        fprintf(stderr, "Can't stat '%s':  %s\n", filename, strerror(errno));
        close(fd);
        return -1;
    }
    text = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    end = text + sb.st_size;
    if (text == MAP_FAILED) {
        fprintf(stderr, "Can't mmap '%s':  %s\n", filename, strerror(errno));
        close(fd);
        return -1;
    }
    close(fd);
    return check_neverallows(policydb, text, end);
}

static int check_neverallows_string(policydb_t *policydb, char *string, size_t len)
{
    char *text, *end;
    text = string;
    end = text + len;
    return check_neverallows(policydb, text, end);
}

int neverallow_func (int argc, char **argv, policydb_t *policydb) {
    char *rules = 0, *file = 0;
    char ch;

    struct option neverallow_options[] = {
        {"debug", no_argument, NULL, 'd'},
        {"file_input", required_argument, NULL, 'f'},
        {"neverallow", required_argument, NULL, 'n'},
        {"warn", no_argument, NULL, 'w'},
        {NULL, 0, NULL, 0}
    };

    while ((ch = getopt_long(argc, argv, "df:n:w", neverallow_options, NULL)) != -1) {
        switch (ch) {
        case 'd':
            debug = 1;
            break;
        case 'f':
            file = optarg;
            break;
        case 'n':
            rules = optarg;
            break;
        case 'w':
            warn = 1;
            break;
        default:
            USAGE_ERROR = true;
            return -1;
        }
    }

    if (!(rules || file) || (rules && file)){
        USAGE_ERROR = true;
        return -1;
    }
    if (file) {
        return check_neverallows_file(policydb, file);
    } else {
        return check_neverallows_string(policydb, rules, strlen(rules));
    }
}
