#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sepol/module.h>
#include <sepol/policydb/policydb.h>
#include <sepol/sepol.h>
#include <selinux/selinux.h>
#include <selinux/label.h>
#include <sys/stat.h>
#include <sys/types.h>

static const char * const CHECK_FC_ASSERT_ATTRS[] = { "fs_type", "dev_type", "file_type", NULL };
static const char * const CHECK_PC_ASSERT_ATTRS[] = { "property_type", NULL };
static const char * const CHECK_SC_ASSERT_ATTRS[] = { "service_manager_type", NULL };
static const char * const CHECK_HW_SC_ASSERT_ATTRS[] = { "hwservice_manager_type", NULL };
static const char * const CHECK_VND_SC_ASSERT_ATTRS[] = { "vndservice_manager_type", NULL };

typedef enum filemode filemode;
enum filemode {
    filemode_file_contexts = 0,
    filemode_property_contexts,
    filemode_service_contexts,
    filemode_hw_service_contexts,
    filemode_vendor_service_contexts
};

static struct {
    /* policy */
    struct {
        union {
            /* Union these so we don't have to cast */
            sepol_policydb_t *sdb;
            policydb_t *pdb;
        };
        sepol_policy_file_t *pf;
        sepol_handle_t *handle;
        FILE *file;
#define SEHANDLE_CNT 2
        struct selabel_handle *sehnd[SEHANDLE_CNT];
    } sepolicy;

    /* assertions */
    struct {
        const char * const *attrs; /* for the original set to print on error */
        ebitmap_t set;             /* the ebitmap representation of the attrs */
    } assert;

} global_state;

static const char * const *filemode_to_assert_attrs(filemode mode)
{
    switch (mode) {
    case filemode_file_contexts:
        return CHECK_FC_ASSERT_ATTRS;
    case filemode_property_contexts:
        return CHECK_PC_ASSERT_ATTRS;
    case filemode_service_contexts:
        return CHECK_SC_ASSERT_ATTRS;
    case filemode_hw_service_contexts:
        return CHECK_HW_SC_ASSERT_ATTRS;
    case filemode_vendor_service_contexts:
        return CHECK_VND_SC_ASSERT_ATTRS;
    }
    /* die on invalid parameters */
    fprintf(stderr, "Error: Invalid mode of operation: %d\n", mode);
    exit(1);
}

static int get_attr_bit(policydb_t *policydb, const char *attr_name)
{
    struct type_datum *attr = hashtab_search(policydb->p_types.table, (char *)attr_name);
    if (!attr) {
        fprintf(stderr, "Error: \"%s\" is not defined in this policy.\n", attr_name);
        return -1;
    }

    if (attr->flavor != TYPE_ATTRIB) {
        fprintf(stderr, "Error: \"%s\" is not an attribute in this policy.\n", attr_name);
        return -1;
    }

    return attr->s.value - 1;
}

static bool ebitmap_attribute_assertion_init(ebitmap_t *assertions, const char * const attributes[])
{

    while (*attributes) {

        int bit_pos = get_attr_bit(global_state.sepolicy.pdb, *attributes);
        if (bit_pos < 0) {
            /* get_attr_bit() logs error */
            return false;
        }

        int err = ebitmap_set_bit(assertions, bit_pos, 1);
        if (err) {
            fprintf(stderr, "Error: setting bit on assertion ebitmap!\n");
            return false;
        }
        attributes++;
    }
    return true;
}

static bool is_type_of_attribute_set(policydb_t *policydb, const char *type_name,
        ebitmap_t *attr_set)
{
    struct type_datum *type = hashtab_search(policydb->p_types.table, (char *)type_name);
    if (!type) {
        fprintf(stderr, "Error: \"%s\" is not defined in this policy.\n", type_name);
        return false;
    }

    if (type->flavor != TYPE_TYPE) {
        fprintf(stderr, "Error: \"%s\" is not a type in this policy.\n", type_name);
        return false;
    }

    ebitmap_t dst;
    ebitmap_init(&dst);

    /* Take the intersection, if the set is empty, then its a failure */
    int rc = ebitmap_and(&dst, attr_set, &policydb->type_attr_map[type->s.value - 1]);
    if (rc) {
        fprintf(stderr, "Error: Could not perform ebitmap_and: %d\n", rc);
        exit(1);
    }

    bool res = (bool)ebitmap_length(&dst);

    ebitmap_destroy(&dst);
    return res;
}

static void dump_char_array(FILE *stream, const char * const *strings)
{

    const char * const *p = strings;

    fprintf(stream, "\"");

    while (*p) {
        const char *s = *p++;
        const char *fmt = *p ? "%s, " : "%s\"";
        fprintf(stream, fmt, s);
    }
}

static int validate(char **contextp)
{
    bool res;
    char *context = *contextp;

    sepol_context_t *ctx;
    int rc = sepol_context_from_string(global_state.sepolicy.handle, context,
            &ctx);
    if (rc < 0) {
        fprintf(stderr, "Error: Could not allocate context from string");
        exit(1);
    }

    rc = sepol_context_check(global_state.sepolicy.handle,
            global_state.sepolicy.sdb, ctx);
    if (rc < 0) {
        goto out;
    }

    const char *type_name = sepol_context_get_type(ctx);

    uint32_t len = ebitmap_length(&global_state.assert.set);
    if (len > 0) {
        res = !is_type_of_attribute_set(global_state.sepolicy.pdb, type_name,
                &global_state.assert.set);
        if (res) {
            fprintf(stderr, "Error: type \"%s\" is not of set: ", type_name);
            dump_char_array(stderr, global_state.assert.attrs);
            fprintf(stderr, "\n");
            /* The calls above did not affect rc, so set error before going to out */
            rc = -1;
            goto out;
        }
    }
    /* Success: Although it should be 0, we explicitly set rc to 0 for clarity */
    rc = 0;

 out:
    sepol_context_free(ctx);
    return rc;
}

static void usage(char *name) {
    fprintf(stderr, "usage1:  %s [-l|-p|-s|-v] [-e] sepolicy context_file\n\n"
        "Parses a context file and checks for syntax errors.\n"
        "If -p is specified, the property backend is used.\n"
        "If -s is specified, the service backend is used to verify binder services.\n"
        "If -l is specified, the service backend is used to verify hwbinder services.\n"
        "If -v is specified, the service backend is used to verify vndbinder services.\n"
        "Otherwise, context_file is assumed to be a file_contexts file\n"
        "If -e is specified, then the context_file is allowed to be empty.\n\n"

        "usage2:  %s -c file_contexts1 file_contexts2\n\n"
        "Compares two file contexts files and reports one of subset, equal, superset, or incomparable.\n\n",
        name, name);
    exit(1);
}

static void cleanup(void) {

    if (global_state.sepolicy.file) {
        fclose(global_state.sepolicy.file);
    }

    if (global_state.sepolicy.sdb) {
        sepol_policydb_free(global_state.sepolicy.sdb);
    }

    if (global_state.sepolicy.pf) {
        sepol_policy_file_free(global_state.sepolicy.pf);
    }

    if (global_state.sepolicy.handle) {
        sepol_handle_destroy(global_state.sepolicy.handle);
    }

    ebitmap_destroy(&global_state.assert.set);

    int i;
    for (i = 0; i < SEHANDLE_CNT; i++) {
        struct selabel_handle *sehnd = global_state.sepolicy.sehnd[i];
        if (sehnd) {
            selabel_close(sehnd);
        }
    }
}

static void do_compare_and_die_on_error(struct selinux_opt opts[], unsigned int backend, char *paths[])
{
    enum selabel_cmp_result result;
     char *result_str[] = { "subset", "equal", "superset", "incomparable" };
     int i;

     opts[0].value = NULL; /* not validating against a policy when comparing */

     for (i = 0; i < SEHANDLE_CNT; i++) {
         opts[1].value = paths[i];
         global_state.sepolicy.sehnd[i] = selabel_open(backend, opts, 2);
         if (!global_state.sepolicy.sehnd[i]) {
             fprintf(stderr, "Error: could not load context file from %s\n", paths[i]);
             exit(1);
         }
     }

     result = selabel_cmp(global_state.sepolicy.sehnd[0], global_state.sepolicy.sehnd[1]);
     printf("%s\n", result_str[result]);
}

static void do_fc_check_and_die_on_error(struct selinux_opt opts[], unsigned int backend, filemode mode,
        const char *sepolicy_file, const char *context_file, bool allow_empty)
{
    struct stat sb;
    if (stat(context_file, &sb) < 0) {
        perror("Error: could not get stat on file contexts file");
        exit(1);
    }

    if (sb.st_size == 0) {
        /* Nothing to check on empty file_contexts file if allowed*/
        if (allow_empty) {
            return;
        }
        /* else: We could throw the error here, but libselinux backend will catch it */
    }

    global_state.sepolicy.file = fopen(sepolicy_file, "r");
    if (!global_state.sepolicy.file) {
      perror("Error: could not open policy file");
      exit(1);
    }

    global_state.sepolicy.handle = sepol_handle_create();
    if (!global_state.sepolicy.handle) {
        fprintf(stderr, "Error: could not create policy handle: %s\n", strerror(errno));
        exit(1);
    }

    if (sepol_policy_file_create(&global_state.sepolicy.pf) < 0) {
      perror("Error: could not create policy handle");
      exit(1);
    }

    sepol_policy_file_set_fp(global_state.sepolicy.pf, global_state.sepolicy.file);
    sepol_policy_file_set_handle(global_state.sepolicy.pf, global_state.sepolicy.handle);

    int rc = sepol_policydb_create(&global_state.sepolicy.sdb);
    if (rc < 0) {
      perror("Error: could not create policy db");
      exit(1);
    }

    rc = sepol_policydb_read(global_state.sepolicy.sdb, global_state.sepolicy.pf);
    if (rc < 0) {
      perror("Error: could not read file into policy db");
      exit(1);
    }

    global_state.assert.attrs = filemode_to_assert_attrs(mode);

    bool ret = ebitmap_attribute_assertion_init(&global_state.assert.set, global_state.assert.attrs);
    if (!ret) {
        /* error messages logged by ebitmap_attribute_assertion_init() */
        exit(1);
    }

    selinux_set_callback(SELINUX_CB_VALIDATE,
                         (union selinux_callback)&validate);

    opts[1].value = context_file;

    global_state.sepolicy.sehnd[0] = selabel_open(backend, opts, 2);
    if (!global_state.sepolicy.sehnd[0]) {
      fprintf(stderr, "Error: could not load context file from %s\n", context_file);
      exit(1);
    }
}

int main(int argc, char **argv)
{
  struct selinux_opt opts[] = {
    { SELABEL_OPT_VALIDATE, (void*)1 },
    { SELABEL_OPT_PATH, NULL }
  };

  // Default backend unless changed by input argument.
  unsigned int backend = SELABEL_CTX_FILE;

  bool allow_empty = false;
  bool compare = false;
  char c;

  filemode mode = filemode_file_contexts;

  while ((c = getopt(argc, argv, "clpsve")) != -1) {
    switch (c) {
      case 'c':
        compare = true;
        break;
      case 'e':
        allow_empty = true;
        break;
      case 'p':
        mode = filemode_property_contexts;
        backend = SELABEL_CTX_ANDROID_PROP;
        break;
      case 's':
        mode = filemode_service_contexts;
        backend = SELABEL_CTX_ANDROID_SERVICE;
        break;
      case 'l':
        mode = filemode_hw_service_contexts;
        backend = SELABEL_CTX_ANDROID_SERVICE;
        break;
      case 'v':
        mode = filemode_vendor_service_contexts;
        backend = SELABEL_CTX_ANDROID_SERVICE;
        break;
      case 'h':
      default:
        usage(argv[0]);
        break;
    }
  }

  int index = optind;
  if (argc - optind != 2) {
    usage(argv[0]);
  }

  if (compare && backend != SELABEL_CTX_FILE) {
    usage(argv[0]);
  }

  atexit(cleanup);

  if (compare) {
      do_compare_and_die_on_error(opts, backend, &(argv[index]));
  } else {
      /* remaining args are sepolicy file and context file  */
      char *sepolicy_file = argv[index];
      char *context_file = argv[index + 1];

      do_fc_check_and_die_on_error(opts, backend, mode, sepolicy_file, context_file, allow_empty);
  }
  exit(0);
}
