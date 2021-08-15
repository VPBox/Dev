#include <stdio.h>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sepol/policydb/avtab.h>
#include <sepol/policydb/policydb.h>
#include <sepol/policydb/services.h>
#include <sepol/policydb/util.h>
#include <sys/types.h>
#include <fstream>

#include <android-base/file.h>
#include <android-base/strings.h>
#include <sepol_wrap.h>

struct genfs_iter {
    genfs_t *genfs;
    ocontext_t *ocon;
};

void *init_genfs_iter(void *policydbp)
{
    struct genfs_iter *out = (struct genfs_iter *)
                            calloc(1, sizeof(struct genfs_iter));

    if (!out) {
        std::cerr << "Failed to allocate genfs iterator" << std::endl;
        return nullptr;
    }

    policydb_t *db = static_cast<policydb_t *>(policydbp);

    out->genfs = db->genfs;
    out->ocon = db->genfs->head;

    return static_cast<void *>(out);
}

/*
 * print genfs path into *out buffer.
 *
 * Returns -1 on error.
 * Returns 0 on successfully retrieving a genfs entry.
 * Returns 1 on successfully retrieving the final genfs entry.
 */
int get_genfs(char *out, size_t max_size, void *policydbp, void *genfs_iterp)
{
    size_t len;
    struct genfs_iter *i = static_cast<struct genfs_iter *>(genfs_iterp);
    policydb_t *db = static_cast<policydb_t *>(policydbp);

    len = snprintf(out, max_size, "%s %s %s:%s:%s:s0",
            i->genfs->fstype,
            i->ocon->u.name,
            db->p_user_val_to_name[i->ocon->context->user-1],
            db->p_role_val_to_name[i->ocon->context->role-1],
            db->p_type_val_to_name[i->ocon->context->type-1]);

    if (len >= max_size) {
        std::cerr << "genfs path exceeds buffer size." << std::endl;
        return -1;
    }

    i->ocon = i->ocon->next;
    if (i->ocon == nullptr) {
        if (i->genfs->next != nullptr) {
            i->genfs = i->genfs->next;
            i->ocon = i->genfs->head;
        } else {
            return 1;
        }
    }

    return 0;
}

void destroy_genfs_iter(void *genfs_iterp)
{
    struct genfs_iter *genfs_i = static_cast<struct genfs_iter *>(genfs_iterp);
    free(genfs_i);
}

#define TYPE_ITER_LOOKUP   0
#define TYPE_ITER_ALLTYPES 1
#define TYPE_ITER_ALLATTRS 2
struct type_iter {
    unsigned int alltypes;
    type_datum *d;
    ebitmap_node *n;
    unsigned int length;
    unsigned int bit;
};

void *init_type_iter(void *policydbp, const char *type, bool is_attr)
{
    policydb_t *db = static_cast<policydb_t *>(policydbp);
    struct type_iter *out = (struct type_iter *)
                            calloc(1, sizeof(struct type_iter));

    if (!out) {
        std::cerr << "Failed to allocate type type iterator" << std::endl;
        return nullptr;
    }

    if (type == nullptr) {
        out->length = db->p_types.nprim;
        out->bit = 0;
        if (is_attr)
            out->alltypes = TYPE_ITER_ALLATTRS;
        else
            out->alltypes = TYPE_ITER_ALLTYPES;
    } else {
        out->alltypes = TYPE_ITER_LOOKUP;
        out->d = static_cast<type_datum *>(hashtab_search(db->p_types.table, type));
        if (out->d == nullptr) {
            std::cerr << "\"" << type << "\" does not exist" << std::endl;
            free(out);
            return nullptr;
        }
        if (is_attr && out->d->flavor != TYPE_ATTRIB) {
            std::cerr << "\"" << type << "\" MUST be an attribute in the policy" << std::endl;
            free(out);
            return nullptr;
        } else if (!is_attr && out->d->flavor != TYPE_TYPE) {
            std::cerr << "\"" << type << "\" MUST be a type in the policy" << std::endl;
            free(out);
            return nullptr;
        }

        if (is_attr) {
            out->bit = ebitmap_start(&db->attr_type_map[out->d->s.value - 1], &out->n);
            out->length = ebitmap_length(&db->attr_type_map[out->d->s.value - 1]);
        } else {
            out->bit = ebitmap_start(&db->type_attr_map[out->d->s.value - 1], &out->n);
            out->length = ebitmap_length(&db->type_attr_map[out->d->s.value - 1]);
        }
    }

    return static_cast<void *>(out);
}

void destroy_type_iter(void *type_iterp)
{
    struct type_iter *type_i = static_cast<struct type_iter *>(type_iterp);
    free(type_i);
}

/*
 * print type into *out buffer.
 *
 * Returns -1 on error.
 * Returns 0 on successfully reading an avtab entry.
 * Returns 1 on complete
 */
int get_type(char *out, size_t max_size, void *policydbp, void *type_iterp)
{
    size_t len;
    policydb_t *db = static_cast<policydb_t *>(policydbp);
    struct type_iter *i = static_cast<struct type_iter *>(type_iterp);

    if (!i->alltypes) {
        for (; i->bit < i->length; i->bit = ebitmap_next(&i->n, i->bit)) {
            if (!ebitmap_node_get_bit(i->n, i->bit)) {
                continue;
            }
            break;
        }
    }
    while (i->bit < i->length &&
           ((i->alltypes == TYPE_ITER_ALLATTRS
            && db->type_val_to_struct[i->bit]->flavor != TYPE_ATTRIB)
            || (i->alltypes == TYPE_ITER_ALLTYPES
            && db->type_val_to_struct[i->bit]->flavor != TYPE_TYPE))) {
        i->bit++;
    }
    if (i->bit >= i->length)
        return 1;
    len = snprintf(out, max_size, "%s", db->p_type_val_to_name[i->bit]);
    if (len >= max_size) {
        std::cerr << "type name exceeds buffer size." << std::endl;
        return -1;
    }
    i->alltypes ? i->bit++ : i->bit = ebitmap_next(&i->n, i->bit);
    return 0;
}

void *load_policy(const char *policy_path)
{
    FILE *fp;
    policydb_t *db;

    fp = fopen(policy_path, "re");
    if (!fp) {
        std::cerr << "Invalid or non-existing policy file: " << policy_path << std::endl;
        return nullptr;
    }

    db = (policydb_t *) calloc(1, sizeof(policydb_t));
    if (!db) {
        std::cerr << "Failed to allocate memory for policy db." << std::endl;
        fclose(fp);
        return nullptr;
    }

    sidtab_t sidtab;
    sepol_set_sidtab(&sidtab);
    sepol_set_policydb(db);

    struct stat sb;
    if (fstat(fileno(fp), &sb)) {
        std::cerr << "Failed to stat the policy file" << std::endl;
        free(db);
        fclose(fp);
        return nullptr;
    }

    auto unmap = [=](void *ptr) { munmap(ptr, sb.st_size); };
    std::unique_ptr<void, decltype(unmap)> map(
        mmap(nullptr, sb.st_size, PROT_READ, MAP_PRIVATE, fileno(fp), 0), unmap);
    if (!map) {
        std::cerr << "Failed to map the policy file" << std::endl;
        free(db);
        fclose(fp);
        return nullptr;
    }

    struct policy_file pf;
    policy_file_init(&pf);
    pf.type = PF_USE_MEMORY;
    pf.data = static_cast<char *>(map.get());
    pf.len = sb.st_size;
    if (policydb_init(db)) {
        std::cerr << "Failed to initialize policydb" << std::endl;
        free(db);
        fclose(fp);
        return nullptr;
    }

    if (policydb_read(db, &pf, 0)) {
        std::cerr << "Failed to read binary policy" << std::endl;
        policydb_destroy(db);
        free(db);
        fclose(fp);
        return nullptr;
    }

    return static_cast<void *>(db);
}

/* items needed to iterate over the avtab */
struct avtab_iter {
    avtab_t *avtab;
    uint32_t i;
    avtab_ptr_t cur;
};

/*
 * print allow rule into *out buffer.
 *
 * Returns -1 on error.
 * Returns 0 on successfully reading an avtab entry.
 * Returns 1 on complete
 */
static int get_avtab_allow_rule(char *out, size_t max_size, policydb_t *db,
                                 struct avtab_iter *avtab_i)
{
    size_t len;

    for (; avtab_i->i < avtab_i->avtab->nslot; (avtab_i->i)++) {
        if (avtab_i->cur == nullptr) {
            avtab_i->cur = avtab_i->avtab->htable[avtab_i->i];
        }
        for (; avtab_i->cur; avtab_i->cur = (avtab_i->cur)->next) {
            if (!((avtab_i->cur)->key.specified & AVTAB_ALLOWED)) continue;

            len = snprintf(out, max_size, "allow,%s,%s,%s,%s",
                    db->p_type_val_to_name[(avtab_i->cur)->key.source_type - 1],
                    db->p_type_val_to_name[(avtab_i->cur)->key.target_type - 1],
                    db->p_class_val_to_name[(avtab_i->cur)->key.target_class - 1],
                    sepol_av_to_string(db, (avtab_i->cur)->key.target_class, (avtab_i->cur)->datum.data));
            avtab_i->cur = (avtab_i->cur)->next;
            if (!(avtab_i->cur))
                (avtab_i->i)++;
            if (len >= max_size) {
                std::cerr << "Allow rule exceeds buffer size." << std::endl;
                return -1;
            }
            return 0;
        }
        avtab_i->cur = nullptr;
    }

    return 1;
}

int get_allow_rule(char *out, size_t len, void *policydbp, void *avtab_iterp)
{
    policydb_t *db = static_cast<policydb_t *>(policydbp);
    struct avtab_iter *avtab_i = static_cast<struct avtab_iter *>(avtab_iterp);

    return get_avtab_allow_rule(out, len, db, avtab_i);
}

static avtab_iter *init_avtab_common(avtab_t *in)
{
    struct avtab_iter *out = (struct avtab_iter *)
                            calloc(1, sizeof(struct avtab_iter));
    if (!out) {
        std::cerr << "Failed to allocate avtab iterator" << std::endl;
        return nullptr;
    }

    out->avtab = in;
    return out;
}

void *init_avtab(void *policydbp)
{
    policydb_t *p = static_cast<policydb_t *>(policydbp);
    return static_cast<void *>(init_avtab_common(&p->te_avtab));
}

void *init_cond_avtab(void *policydbp)
{
    policydb_t *p = static_cast<policydb_t *>(policydbp);
    return static_cast<void *>(init_avtab_common(&p->te_cond_avtab));
}

void destroy_avtab(void *avtab_iterp)
{
    struct avtab_iter *avtab_i = static_cast<struct avtab_iter *>(avtab_iterp);
    free(avtab_i);
}

/*
 * <sepol/policydb/expand.h->conditional.h> uses 'bool' as a variable name
 * inside extern "C" { .. } construct, which clang doesn't like.
 * So, declare the function we need from expand.h ourselves.
 */
extern "C" int expand_avtab(policydb_t *p, avtab_t *a, avtab_t *expa);

static avtab_iter *init_expanded_avtab_common(avtab_t *in, policydb_t *p)
{
    struct avtab_iter *out = (struct avtab_iter *)
                            calloc(1, sizeof(struct avtab_iter));
    if (!out) {
        std::cerr << "Failed to allocate avtab iterator" << std::endl;
        return nullptr;
    }

    avtab_t *avtab = (avtab_t *) calloc(1, sizeof(avtab_t));

    if (!avtab) {
        std::cerr << "Failed to allocate avtab" << std::endl;
        free(out);
        return nullptr;
    }

    out->avtab = avtab;
    if (avtab_init(out->avtab)) {
        std::cerr << "Failed to initialize avtab" << std::endl;
        free(avtab);
        free(out);
        return nullptr;
    }

    if (expand_avtab(p, in, out->avtab)) {
        std::cerr << "Failed to expand avtab" << std::endl;
        free(avtab);
        free(out);
        return nullptr;
    }
    return out;
}

void *init_expanded_avtab(void *policydbp)
{
    policydb_t *p = static_cast<policydb_t *>(policydbp);
    return static_cast<void *>(init_expanded_avtab_common(&p->te_avtab, p));
}

void *init_expanded_cond_avtab(void *policydbp)
{
    policydb_t *p = static_cast<policydb_t *>(policydbp);
    return static_cast<void *>(init_expanded_avtab_common(&p->te_cond_avtab, p));
}

void destroy_expanded_avtab(void *avtab_iterp)
{
    struct avtab_iter *avtab_i = static_cast<struct avtab_iter *>(avtab_iterp);
    avtab_destroy(avtab_i->avtab);
    free(avtab_i->avtab);
    free(avtab_i);
}

void destroy_policy(void *policydbp)
{
    policydb_t *p = static_cast<policydb_t *>(policydbp);
    policydb_destroy(p);
}
