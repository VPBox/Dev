#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <search.h>
#include <stdbool.h>
#include <sepol/sepol.h>
#include <sepol/policydb/policydb.h>
#include <pcre2.h>

#define TABLE_SIZE 1024
#define KVP_NUM_OF_RULES (sizeof(rules) / sizeof(key_map))
#define log_set_verbose() do { logging_verbose = 1; log_info("Enabling verbose\n"); } while(0)
#define log_error(fmt, ...) log_msg(stderr, "Error: ", fmt, ##__VA_ARGS__)
#define log_warn(fmt, ...) log_msg(stderr, "Warning: ", fmt, ##__VA_ARGS__)
#define log_info(fmt, ...) if (logging_verbose ) { log_msg(stdout, "Info: ", fmt, ##__VA_ARGS__); }

/**
 * Initializes an empty, static list.
 */
#define list_init(free_fn) { .head = NULL, .tail = NULL, .freefn = (free_fn) }

/**
 * given an item in the list, finds the offset for the container
 * it was stored in.
 *
 * @element The element from the list
 * @type The container type ie what you allocated that has the list_element structure in it.
 * @name The name of the field that is the list_element
 *
 */
#define list_entry(element, type, name) \
		(type *)(((uint8_t *)(element)) - (uint8_t *)&(((type *)NULL)->name))

/**
 * Iterates over the list, do not free elements from the list when using this.
 * @list The list head to walk
 * @var The variable name for the cursor
 */
#define list_for_each(list, var) \
	for(var = (list)->head; var != NULL; var = var->next) /*NOLINT*/


typedef struct hash_entry hash_entry;
typedef enum key_dir key_dir;
typedef enum data_type data_type;
typedef enum rule_map_switch rule_map_switch;
typedef enum map_match map_match;
typedef struct key_map key_map;
typedef struct kvp kvp;
typedef struct rule_map rule_map;
typedef struct policy_info policy_info;
typedef struct list_element list_element;
typedef struct list list;
typedef struct key_map_regex key_map_regex;
typedef struct file_info file_info;

enum map_match {
	map_no_matches,
	map_input_matched,
	map_matched
};

const char *map_match_str[] = {
	"do not match",
	"match on all inputs",
	"match on everything"
};

/**
 * Whether or not the "key" from a key vaue pair is considered an
 * input or an output.
 */
enum key_dir {
	dir_in, dir_out
};

struct list_element {
	list_element *next;
};

struct list {
	list_element *head;
	list_element *tail;
	void (*freefn)(list_element *e);
};

struct key_map_regex {
	pcre2_code *compiled;
	pcre2_match_data *match_data;
};

/**
 * The workhorse of the logic. This struct maps key value pairs to
 * an associated set of meta data maintained in rule_map_new()
 */
struct key_map {
	char *name;
	key_dir dir;
	char *data;
	key_map_regex regex;
	bool (*fn_validate)(char *value, char **errmsg);
};

/**
 * Key value pair struct, this represents the raw kvp values coming
 * from the rules files.
 */
struct kvp {
	char *key;
	char *value;
};

/**
 * Rules are made up of meta data and an associated set of kvp stored in a
 * key_map array.
 */
struct rule_map {
	bool is_never_allow;
	list violations;
	list_element listify;
	char *key; /** key value before hashing */
	size_t length; /** length of the key map */
	int lineno; /** Line number rule was encounter on */
	char *filename; /** File it was found in */
	key_map m[]; /** key value mapping */
};

struct hash_entry {
	list_element listify;
	rule_map *r; /** The rule map to store at that location */
};

/**
 * Data associated for a policy file
 */
struct policy_info {

	char *policy_file_name; /** policy file path name */
	FILE *policy_file;      /** file handle to the policy file */
	sepol_policydb_t *db;
	sepol_policy_file_t *pf;
	sepol_handle_t *handle;
	sepol_context_t *con;
};

struct file_info {
	FILE *file; /** file itself */
	const char *name; /** name of file. do not free, these are not alloc'd */
	list_element listify;
};

static void input_file_list_freefn(list_element *e);
static void line_order_list_freefn(list_element *e);
static void rule_map_free(rule_map *rm, bool is_in_htable);

/** Set to !0 to enable verbose logging */
static int logging_verbose = 0;

/** file handle to the output file */
static file_info out_file;

static list input_file_list = list_init(input_file_list_freefn);

static policy_info pol = {
	.policy_file_name = NULL,
	.policy_file = NULL,
	.db = NULL,
	.pf = NULL,
	.handle = NULL,
	.con = NULL
};

/**
 * Head pointer to a linked list of
 * rule map table entries (hash_entry), used for
 * preserving the order of entries
 * based on "first encounter"
 */
static list line_order_list = list_init(line_order_list_freefn);

/*
 * List of hash_entrys for never allow rules.
 */
static list nallow_list = list_init(line_order_list_freefn);

/* validation call backs */
static bool validate_bool(char *value, char **errmsg);
static bool validate_levelFrom(char *value, char **errmsg);
static bool validate_selinux_type(char *value, char **errmsg);
static bool validate_selinux_level(char *value, char **errmsg);
static bool validate_uint(char *value, char **errmsg);

/**
 * The heart of the mapping process, this must be updated if a new key value pair is added
 * to a rule.
 */
key_map rules[] = {
                /*Inputs*/
                { .name = "isSystemServer", .dir = dir_in, .fn_validate = validate_bool },
                { .name = "isEphemeralApp",  .dir = dir_in, .fn_validate = validate_bool },
                { .name = "isOwner",        .dir = dir_in, .fn_validate = validate_bool },
                { .name = "user",           .dir = dir_in,                              },
                { .name = "seinfo",         .dir = dir_in,                              },
                { .name = "name",           .dir = dir_in,                              },
                { .name = "path",           .dir = dir_in,                              },
                { .name = "isPrivApp",      .dir = dir_in, .fn_validate = validate_bool },
                { .name = "minTargetSdkVersion", .dir = dir_in, .fn_validate = validate_uint },
                { .name = "fromRunAs",       .dir = dir_in, .fn_validate = validate_bool },
                /*Outputs*/
                { .name = "domain",         .dir = dir_out, .fn_validate = validate_selinux_type  },
                { .name = "type",           .dir = dir_out, .fn_validate = validate_selinux_type  },
                { .name = "levelFromUid",   .dir = dir_out, .fn_validate = validate_bool          },
                { .name = "levelFrom",      .dir = dir_out, .fn_validate = validate_levelFrom     },
                { .name = "level",          .dir = dir_out, .fn_validate = validate_selinux_level },
};

/**
 * Appends to the end of the list.
 * @list The list to append to
 * @e the element to append
 */
void list_append(list *list, list_element *e) {

	memset(e, 0, sizeof(*e));

	if (list->head == NULL ) {
		list->head = list->tail = e;
		return;
	}

	list->tail->next = e;
	list->tail = e;
	return;
}

/**
 * Free's all the elements in the specified list.
 * @list The list to free
 */
static void list_free(list *list) {

	list_element *tmp;
	list_element *cursor = list->head;

	while (cursor) {
		tmp = cursor;
		cursor = cursor->next;
		if (list->freefn) {
			list->freefn(tmp);
		}
	}
}

/*
 * called when the lists are freed
 */
static void line_order_list_freefn(list_element *e) {
	hash_entry *h = list_entry(e, typeof(*h), listify);
	rule_map_free(h->r, true);
	free(h);
}

static void input_file_list_freefn(list_element *e) {
	file_info *f = list_entry(e, typeof(*f), listify);

	if (f->file) {
		fclose(f->file);
	}
	free(f);
}

/**
 * Send a logging message to a file
 * @param out
 * 	Output file to send message too
 * @param prefix
 * 	A special prefix to write to the file, such as "Error:"
 * @param fmt
 * 	The printf style formatter to use, such as "%d"
 */
static void __attribute__ ((format(printf, 3, 4)))
log_msg(FILE *out, const char *prefix, const char *fmt, ...) {

	fprintf(out, "%s", prefix);
	va_list args;
	va_start(args, fmt);
	vfprintf(out, fmt, args);
	va_end(args);
}

/**
 * Checks for a type in the policy.
 * @param db
 * 	The policy db to search
 * @param type
 * 	The type to search for
 * @return
 * 	1 if the type is found, 0 otherwise.
 * @warning
 * 	This function always returns 1 if libsepol is not linked
 * 	statically to this executable and LINK_SEPOL_STATIC is not
 * 	defined.
 */
static int check_type(sepol_policydb_t *db, char *type) {

	int rc = 1;
#if defined(LINK_SEPOL_STATIC)
	policydb_t *d = (policydb_t *)db;
	hashtab_datum_t dat;
	dat = hashtab_search(d->p_types.table, type);
	rc = (dat == NULL) ? 0 : 1;
#endif
	return rc;
}

static bool match_regex(key_map *assert, const key_map *check) {

	char *tomatch = check->data;

	int ret = pcre2_match(assert->regex.compiled, (PCRE2_SPTR) tomatch,
				PCRE2_ZERO_TERMINATED, 0, 0,
				assert->regex.match_data, NULL);

	/* ret > 0 from pcre2_match means matched */
	return ret > 0;
}

static bool compile_regex(key_map *km, int *errcode, PCRE2_SIZE *erroff) {

	size_t size;
	char *anchored;

	/*
	 * Explicitly anchor all regex's
	 * The size is the length of the string to anchor (km->data), the anchor
	 * characters ^ and $ and the null byte. Hence strlen(km->data) + 3
	 */
	size = strlen(km->data) + 3;
	anchored = alloca(size);
	sprintf(anchored, "^%s$", km->data);

	km->regex.compiled = pcre2_compile((PCRE2_SPTR) anchored,
						PCRE2_ZERO_TERMINATED,
						PCRE2_DOTALL,
						errcode, erroff,
						NULL);
	if (!km->regex.compiled) {
		return false;
	}

	km->regex.match_data = pcre2_match_data_create_from_pattern(
			km->regex.compiled, NULL);
	if (!km->regex.match_data) {
		pcre2_code_free(km->regex.compiled);
		return false;
	}
	return true;
}

static bool validate_bool(char *value, char **errmsg) {

	if (!strcmp("true", value) || !strcmp("false", value)) {
		return true;
	}

	*errmsg = "Expecting \"true\" or \"false\"";
	return false;
}

static bool validate_levelFrom(char *value, char **errmsg) {

	if(strcasecmp(value, "none") && strcasecmp(value, "all") &&
		strcasecmp(value, "app") && strcasecmp(value, "user")) {
		*errmsg = "Expecting one of: \"none\", \"all\", \"app\" or \"user\"";
		return false;
	}
	return true;
}

static bool validate_selinux_type(char *value, char **errmsg) {

	/*
	 * No policy file present means we cannot check
	 * SE Linux types
	 */
	if (!pol.policy_file) {
		return true;
	}

	if(!check_type(pol.db, value)) {
		*errmsg = "Expecting a valid SELinux type";
		return false;
	}

	return true;
}

static bool validate_selinux_level(char *value, char **errmsg) {

	/*
	 * No policy file present means we cannot check
	 * SE Linux MLS
	 */
	if (!pol.policy_file) {
		return true;
	}

	int ret = sepol_mls_check(pol.handle, pol.db, value);
	if (ret < 0) {
		*errmsg = "Expecting a valid SELinux MLS value";
		return false;
	}

	return true;
}

static bool validate_uint(char *value, char **errmsg) {

	char *endptr;
	long longvalue;
	longvalue = strtol(value, &endptr, 10);
	if (('\0' != *endptr) || (longvalue < 0) || (longvalue > INT32_MAX)) {
		*errmsg = "Expecting a valid unsigned integer";
		return false;
	}

	return true;
}

/**
 * Validates a key_map against a set of enforcement rules, this
 * function exits the application on a type that cannot be properly
 * checked
 *
 * @param m
 * 	The key map to check
 * @param lineno
 * 	The line number in the source file for the corresponding key map
 * @return
 * 	true if valid, false if invalid
 */
static bool key_map_validate(key_map *m, const char *filename, int lineno,
		bool is_neverallow) {

	PCRE2_SIZE erroff;
	int errcode;
	bool rc = true;
	char *key = m->name;
	char *value = m->data;
	char *errmsg = NULL;
	char errstr[256];

	log_info("Validating %s=%s\n", key, value);

	/*
	 * Neverallows are completely skipped from sanity checking so you can match
	 * un-unspecified inputs.
	 */
	if (is_neverallow) {
		if (!m->regex.compiled) {
			rc = compile_regex(m, &errcode, &erroff);
			if (!rc) {
				pcre2_get_error_message(errcode,
							(PCRE2_UCHAR*) errstr,
							sizeof(errstr));
				log_error("Invalid regex on line %d : %s PCRE error: %s at offset %lu",
						lineno, value, errstr, erroff);
			}
		}
		goto out;
	}

	/* If the key has a validation routine, call it */
	if (m->fn_validate) {
		rc = m->fn_validate(value, &errmsg);

		if (!rc) {
			log_error("Could not validate key \"%s\" for value \"%s\" on line: %d in file: \"%s\": %s\n", key, value,
			lineno, filename, errmsg);
		}
	}

out:
	log_info("Key map validate returning: %d\n", rc);
	return rc;
}

/**
 * Prints a rule map back to a file
 * @param fp
 * 	The file handle to print too
 * @param r
 * 	The rule map to print
 */
static void rule_map_print(FILE *fp, rule_map *r) {

	size_t i;
	key_map *m;

	for (i = 0; i < r->length; i++) {
		m = &(r->m[i]);
		if (i < r->length - 1)
			fprintf(fp, "%s=%s ", m->name, m->data);
		else
			fprintf(fp, "%s=%s", m->name, m->data);
	}
}

/**
 * Compare two rule maps for equality
 * @param rmA
 * 	a rule map to check
 * @param rmB
 * 	a rule map to check
 * @return
 *  a map_match enum indicating the result
 */
static map_match rule_map_cmp(rule_map *rmA, rule_map *rmB) {

	size_t i;
	size_t j;
	int inputs_found = 0;
	int num_of_matched_inputs = 0;
	int input_mode = 0;
	size_t matches = 0;
	key_map *mA;
	key_map *mB;

	for (i = 0; i < rmA->length; i++) {
		mA = &(rmA->m[i]);

		for (j = 0; j < rmB->length; j++) {
			mB = &(rmB->m[j]);
			input_mode = 0;

			if (strcmp(mA->name, mB->name))
				continue;

			if (strcmp(mA->data, mB->data))
				continue;

			if (mB->dir != mA->dir)
				continue;
			else if (mB->dir == dir_in) {
				input_mode = 1;
				inputs_found++;
			}

			if (input_mode) {
				log_info("Matched input lines: name=%s data=%s\n", mA->name, mA->data);
				num_of_matched_inputs++;
			}

			/* Match found, move on */
			log_info("Matched lines: name=%s data=%s", mA->name, mA->data);
			matches++;
			break;
		}
	}

	/* If they all matched*/
	if (matches == rmA->length) {
		log_info("Rule map cmp MATCH\n");
		return map_matched;
	}

	/* They didn't all match but the input's did */
	else if (num_of_matched_inputs == inputs_found) {
		log_info("Rule map cmp INPUT MATCH\n");
		return map_input_matched;
	}

	/* They didn't all match, and the inputs didn't match, ie it didn't
	 * match */
	else {
		log_info("Rule map cmp NO MATCH\n");
		return map_no_matches;
	}
}

/**
 * Frees a rule map
 * @param rm
 * 	rule map to be freed.
 * @is_in_htable
 * 	True if the rule map has been added to the hash table, false
 * 	otherwise.
 */
static void rule_map_free(rule_map *rm, bool is_in_htable) {

	size_t i;
	size_t len = rm->length;
	for (i = 0; i < len; i++) {
		key_map *m = &(rm->m[i]);
		free(m->data);

		if (m->regex.compiled) {
			pcre2_code_free(m->regex.compiled);
		}

		if (m->regex.match_data) {
			pcre2_match_data_free(m->regex.match_data);
		}
	}

	/*
	 * hdestroy() frees comparsion keys for non glibc
	 * on GLIBC we always free on NON-GLIBC we free if
	 * it is not in the htable.
	 */
	if (rm->key) {
#ifdef __GLIBC__
		/* silence unused warning */
		(void)is_in_htable;
		free(rm->key);
#else
		if (!is_in_htable) {
			free(rm->key);
		}
#endif
	}

	free(rm->filename);
	free(rm);
}

static void free_kvp(kvp *k) {
	free(k->key);
	free(k->value);
}

/**
 * Checks a rule_map for any variation of KVP's that shouldn't be allowed.
 * It builds an assertion failure list for each rule map.
 * Note that this function logs all errors.
 *
 * Current Checks:
 * 1. That a specified name entry should have a specified seinfo entry as well.
 * 2. That no rule violates a neverallow
 * @param rm
 *  The rule map to check for validity.
 */
static void rule_map_validate(rule_map *rm) {

	size_t i, j;
	const key_map *rule;
	key_map *nrule;
	hash_entry *e;
	rule_map *assert;
	list_element *cursor;

	list_for_each(&nallow_list, cursor) {
		e = list_entry(cursor, typeof(*e), listify);
		assert = e->r;

		size_t cnt = 0;

		for (j = 0; j < assert->length; j++) {
			nrule = &(assert->m[j]);

			// mark that nrule->name is for a null check
			bool is_null_check = !strcmp(nrule->data, "\"\"");

			for (i = 0; i < rm->length; i++) {
				rule = &(rm->m[i]);

				if (!strcmp(rule->name, nrule->name)) {

					/* the name was found, (data cannot be false) then it was specified */
					is_null_check = false;

					if (match_regex(nrule, rule)) {
						cnt++;
					}
				}
			}

			/*
			 * the nrule was marked in a null check and we never found a match on nrule, thus
			 * it matched and we update the cnt
			 */
			if (is_null_check) {
				cnt++;
			}
		}
		if (cnt == assert->length) {
			list_append(&rm->violations, &assert->listify);
		}
	}
}

/**
 * Given a set of key value pairs, this will construct a new rule map.
 * On error this function calls exit.
 * @param keys
 * 	Keys from a rule line to map
 * @param num_of_keys
 * 	The length of the keys array
 * @param lineno
 * 	The line number the keys were extracted from
 * @return
 * 	A rule map pointer.
 */
static rule_map *rule_map_new(kvp keys[], size_t num_of_keys, int lineno,
		const char *filename, bool is_never_allow) {

	size_t i = 0, j = 0;
	rule_map *new_map = NULL;
	kvp *k = NULL;
	key_map *r = NULL, *x = NULL;
	bool seen[KVP_NUM_OF_RULES];

	for (i = 0; i < KVP_NUM_OF_RULES; i++)
		seen[i] = false;

	new_map = calloc(1, (num_of_keys * sizeof(key_map)) + sizeof(rule_map));
	if (!new_map)
		goto oom;

	new_map->is_never_allow = is_never_allow;
	new_map->length = num_of_keys;
	new_map->lineno = lineno;
	new_map->filename = strdup(filename);
	if (!new_map->filename) {
		goto oom;
	}

	/* For all the keys in a rule line*/
	for (i = 0; i < num_of_keys; i++) {
		k = &(keys[i]);
		r = &(new_map->m[i]);

		for (j = 0; j < KVP_NUM_OF_RULES; j++) {
			x = &(rules[j]);

			/* Only assign key name to map name */
			if (strcasecmp(k->key, x->name)) {
				if (j == KVP_NUM_OF_RULES - 1) {
					log_error("No match for key: %s\n", k->key);
					goto err;
				}
				continue;
			}

			if (seen[j]) {
					log_error("Duplicated key: %s\n", k->key);
					goto err;
			}
			seen[j] = true;

			memcpy(r, x, sizeof(key_map));

			/* Assign rule map value to one from file */
			r->data = strdup(k->value);
			if (!r->data)
				goto oom;

			/* Enforce type check*/
			log_info("Validating keys!\n");
			if (!key_map_validate(r, filename, lineno, new_map->is_never_allow)) {
				log_error("Could not validate\n");
				goto err;
			}

			/*
			 * Only build key off of inputs with the exception of neverallows.
			 * Neverallows are keyed off of all key value pairs,
			 */
			if (r->dir == dir_in || new_map->is_never_allow) {
				char *tmp;
				int key_len = strlen(k->key);
				int val_len = strlen(k->value);
				int l = (new_map->key) ? strlen(new_map->key) : 0;
				l = l + key_len + val_len;
				l += 1;

				tmp = realloc(new_map->key, l);
				if (!tmp)
					goto oom;

				if (!new_map->key)
					memset(tmp, 0, l);

				new_map->key = tmp;

				strncat(new_map->key, k->key, key_len);
				strncat(new_map->key, k->value, val_len);
			}
			break;
		}
		free_kvp(k);
	}

	if (new_map->key == NULL) {
		log_error("Strange, no keys found, input file corrupt perhaps?\n");
		goto err;
	}

	return new_map;

oom:
	log_error("Out of memory!\n");
err:
	if(new_map) {
		rule_map_free(new_map, false);
		for (; i < num_of_keys; i++) {
			k = &(keys[i]);
			free_kvp(k);
		}
	}
	return NULL;
}

/**
 * Print the usage of the program
 */
static void usage() {
	printf(
	        "checkseapp [options] <input file>\n"
		        "Processes an seapp_contexts file specified by argument <input file> (default stdin) "
		        "and allows later declarations to override previous ones on a match.\n"
		        "Options:\n"
		        "-h - print this help message\n"
		        "-v - enable verbose debugging informations\n"
		        "-p policy file - specify policy file for strict checking of output selectors against the policy\n"
		        "-o output file - specify output file or - for stdout. No argument runs in silent mode and outputs nothing\n");
}

static void init() {

	bool has_out_file;
	list_element *cursor;
	file_info *tmp;

	/* input files if the list is empty, use stdin */
	if (!input_file_list.head) {
		log_info("Using stdin for input\n");
		tmp = malloc(sizeof(*tmp));
		if (!tmp) {
			log_error("oom");
			exit(EXIT_FAILURE);
		}
		tmp->name = "stdin";
		tmp->file = stdin;
		list_append(&input_file_list, &(tmp->listify));
	}
	else {
		list_for_each(&input_file_list, cursor) {
			tmp = list_entry(cursor, typeof(*tmp), listify);

			log_info("Opening input file: \"%s\"\n", tmp->name);
			tmp->file = fopen(tmp->name, "r");
			if (!tmp->file) {
				log_error("Could not open file: %s error: %s\n", tmp->name,
						strerror(errno));
				exit(EXIT_FAILURE);
			}
		}
	}

	has_out_file = out_file.name != NULL;

	/* If output file is -, then use stdout, else open the path */
	if (has_out_file && !strcmp(out_file.name, "-")) {
		out_file.file = stdout;
		out_file.name = "stdout";
	}
	else if (has_out_file) {
		out_file.file = fopen(out_file.name, "w+");
	}

	if (has_out_file && !out_file.file) {
		log_error("Could not open file: \"%s\" error: \"%s\"\n", out_file.name,
				strerror(errno));
		exit(EXIT_FAILURE);
	}

	if (pol.policy_file_name) {
		log_info("Opening policy file: %s\n", pol.policy_file_name);
		pol.policy_file = fopen(pol.policy_file_name, "rb");
		if (!pol.policy_file) {
			log_error("Could not open file: %s error: %s\n",
					pol.policy_file_name, strerror(errno));
			exit(EXIT_FAILURE);
		}

		pol.handle = sepol_handle_create();
		if (!pol.handle) {
			log_error("Could not create sepolicy handle: %s\n",
					strerror(errno));
			exit(EXIT_FAILURE);
		}

		if (sepol_policy_file_create(&pol.pf) < 0) {
			log_error("Could not create sepolicy file: %s!\n",
					strerror(errno));
			exit(EXIT_FAILURE);
		}

		sepol_policy_file_set_fp(pol.pf, pol.policy_file);
		sepol_policy_file_set_handle(pol.pf, pol.handle);

		if (sepol_policydb_create(&pol.db) < 0) {
			log_error("Could not create sepolicy db: %s!\n",
					strerror(errno));
			exit(EXIT_FAILURE);
		}

		if (sepol_policydb_read(pol.db, pol.pf) < 0) {
			log_error("Could not load policy file to db: invalid input file!\n");
			exit(EXIT_FAILURE);
		}
	}

	list_for_each(&input_file_list, cursor) {
		tmp = list_entry(cursor, typeof(*tmp), listify);
		log_info("Input file set to: \"%s\"\n", tmp->name);
	}

	log_info("Policy file set to: \"%s\"\n",
			(pol.policy_file_name == NULL) ? "None" : pol.policy_file_name);
	log_info("Output file set to: \"%s\"\n", out_file.name);

#if !defined(LINK_SEPOL_STATIC)
	log_warn("LINK_SEPOL_STATIC is not defined\n""Not checking types!");
#endif

}

/**
 * Handle parsing and setting the global flags for the command line
 * options. This function calls exit on failure.
 * @param argc
 * 	argument count
 * @param argv
 * 	argument list
 */
static void handle_options(int argc, char *argv[]) {

	int c;
	file_info *input_file;

	while ((c = getopt(argc, argv, "ho:p:v")) != -1) {
		switch (c) {
		case 'h':
			usage();
			exit(EXIT_SUCCESS);
		case 'o':
			out_file.name = optarg;
			break;
		case 'p':
			pol.policy_file_name = optarg;
			break;
		case 'v':
			log_set_verbose();
			break;
		case '?':
			if (optopt == 'o' || optopt == 'p')
				log_error("Option -%c requires an argument.\n", optopt);
			else if (isprint (optopt))
				log_error("Unknown option `-%c'.\n", optopt);
			else {
				log_error(
						"Unknown option character `\\x%x'.\n",
						optopt);
			}
		default:
			exit(EXIT_FAILURE);
		}
	}

	for (c = optind; c < argc; c++) {

		input_file = calloc(1, sizeof(*input_file));
		if (!input_file) {
			log_error("oom");
			exit(EXIT_FAILURE);
		}
		input_file->name = argv[c];
		list_append(&input_file_list, &input_file->listify);
	}
}

/**
 * Adds a rule to the hash table and to the ordered list if needed.
 * @param rm
 * 	The rule map to add.
 */
static void rule_add(rule_map *rm) {

	map_match cmp;
	ENTRY e;
	ENTRY *f;
	hash_entry *entry;
	hash_entry *tmp;
	list *list_to_addto;

	e.key = rm->key;
	e.data = NULL;

	log_info("Searching for key: %s\n", e.key);
	/* Check to see if it has already been added*/
	f = hsearch(e, FIND);

	/*
	 * Since your only hashing on a partial key, the inputs we need to handle
	 * when you want to override the outputs for a given input set, as well as
	 * checking for duplicate entries.
	 */
	if(f) {
		log_info("Existing entry found!\n");
		tmp = (hash_entry *)f->data;
		cmp = rule_map_cmp(rm, tmp->r);
		log_error("Duplicate line detected in file: %s\n"
			  "Lines %d and %d %s!\n",
			  rm->filename, tmp->r->lineno, rm->lineno,
			  map_match_str[cmp]);
		rule_map_free(rm, false);
		goto err;
	}
	/* It wasn't found, just add the rule map to the table */
	else {

		entry = malloc(sizeof(hash_entry));
		if (!entry)
			goto oom;

		entry->r = rm;
		e.data = entry;

		f = hsearch(e, ENTER);
		if(f == NULL) {
			goto oom;
		}

		/* new entries must be added to the ordered list */
		entry->r = rm;
		list_to_addto = rm->is_never_allow ? &nallow_list : &line_order_list;
		list_append(list_to_addto, &entry->listify);
	}

	return;
oom:
	if (e.key)
		free(e.key);
	if (entry)
		free(entry);
	if (rm)
		free(rm);
	log_error("Out of memory in function: %s\n", __FUNCTION__);
err:
	exit(EXIT_FAILURE);
}

static void parse_file(file_info *in_file) {

	char *p;
	size_t len;
	char *token;
	char *saveptr;
	bool is_never_allow;
	bool found_whitespace;

	size_t lineno = 0;
	char *name = NULL;
	char *value = NULL;
	size_t token_cnt = 0;

	char line_buf[BUFSIZ];
	kvp keys[KVP_NUM_OF_RULES];

	while (fgets(line_buf, sizeof(line_buf) - 1, in_file->file)) {
		lineno++;
		is_never_allow = false;
		found_whitespace = false;
		log_info("Got line %zu\n", lineno);
		len = strlen(line_buf);
		if (line_buf[len - 1] == '\n')
			line_buf[len - 1] = '\0';
		p = line_buf;

		/* neverallow lines must start with neverallow (ie ^neverallow) */
		if (!strncasecmp(p, "neverallow", strlen("neverallow"))) {
			p += strlen("neverallow");
			is_never_allow = true;
		}

		/* strip trailing whitespace skip comments */
		while (isspace(*p)) {
			p++;
			found_whitespace = true;
		}
		if (*p == '#' || *p == '\0')
			continue;

		token = strtok_r(p, " \t", &saveptr);
		if (!token)
			goto err;

		token_cnt = 0;
		memset(keys, 0, sizeof(kvp) * KVP_NUM_OF_RULES);
		while (1) {

			name = token;
			value = strchr(name, '=');
			if (!value)
				goto err;
			*value++ = 0;

			keys[token_cnt].key = strdup(name);
			if (!keys[token_cnt].key)
				goto oom;

			keys[token_cnt].value = strdup(value);
			if (!keys[token_cnt].value)
				goto oom;

			token_cnt++;

			token = strtok_r(NULL, " \t", &saveptr);
			if (!token)
				break;

			if (token_cnt == KVP_NUM_OF_RULES)
				goto oob;

		} /*End token parsing */

		rule_map *r = rule_map_new(keys, token_cnt, lineno, in_file->name, is_never_allow);
		if (!r)
			goto err;
		rule_add(r);

	} /* End file parsing */
	return;

err:
	log_error("Reading file: \"%s\" line: %zu name: \"%s\" value: \"%s\"\n",
		in_file->name, lineno, name, value);
	if(found_whitespace && name && !strcasecmp(name, "neverallow")) {
		log_error("perhaps whitespace before neverallow\n");
	}
	exit(EXIT_FAILURE);
oom:
	log_error("In function %s:  Out of memory\n", __FUNCTION__);
	exit(EXIT_FAILURE);
oob:
	log_error("Reading file: \"%s\" line: %zu reason: the size of key pairs exceeds the MAX(%zu)\n",
		in_file->name, lineno, KVP_NUM_OF_RULES);
	exit(EXIT_FAILURE);
}

/**
 * Parses the seapp_contexts file and neverallow file
 * and adds them to the hash table and ordered list entries
 * when it encounters them.
 * Calls exit on failure.
 */
static void parse() {

	file_info *current;
	list_element *cursor;
	list_for_each(&input_file_list, cursor) {
		current = list_entry(cursor, typeof(*current), listify);
		parse_file(current);
	}
}

static void validate() {

	list_element *cursor, *v;
	bool found_issues = false;
	hash_entry *e;
	rule_map *r;
	list_for_each(&line_order_list, cursor) {
		e = list_entry(cursor, typeof(*e), listify);
		rule_map_validate(e->r);
	}

	list_for_each(&line_order_list, cursor) {
		e = list_entry(cursor, typeof(*e), listify);
		r = e->r;
		list_for_each(&r->violations, v) {
			found_issues = true;
			log_error("Rule in File \"%s\" on line %d: \"", e->r->filename, e->r->lineno);
			rule_map_print(stderr, e->r);
			r = list_entry(v, rule_map, listify);
			fprintf(stderr, "\" violates neverallow in File \"%s\" on line %d: \"", r->filename, r->lineno);
			rule_map_print(stderr, r);
			fprintf(stderr, "\"\n");
		}
	}

	if (found_issues) {
		exit(EXIT_FAILURE);
	}
}

/**
 * Should be called after parsing to cause the printing of the rule_maps
 * stored in the ordered list, head first, which preserves the "first encountered"
 * ordering.
 */
static void output() {

	hash_entry *e;
	list_element *cursor;

	if (!out_file.file) {
		log_info("No output file, not outputting.\n");
		return;
	}

	list_for_each(&line_order_list, cursor) {
		e = list_entry(cursor, hash_entry, listify);
		rule_map_print(out_file.file, e->r);
		fprintf(out_file.file, "\n");
	}
}

/**
 * This function is registered to the at exit handler and should clean up
 * the programs dynamic resources, such as memory and fd's.
 */
static void cleanup() {

	/* Only close this when it was opened by me and not the crt */
	if (out_file.name && strcmp(out_file.name, "stdout") && out_file.file) {
		log_info("Closing file: %s\n", out_file.name);
		fclose(out_file.file);
	}

	if (pol.policy_file) {

		log_info("Closing file: %s\n", pol.policy_file_name);
		fclose(pol.policy_file);

		if (pol.db)
			sepol_policydb_free(pol.db);

		if (pol.pf)
			sepol_policy_file_free(pol.pf);

		if (pol.handle)
			sepol_handle_destroy(pol.handle);
	}

	log_info("Freeing lists\n");
	list_free(&input_file_list);
	list_free(&line_order_list);
	list_free(&nallow_list);
	hdestroy();
}

int main(int argc, char *argv[]) {
	if (!hcreate(TABLE_SIZE)) {
		log_error("Could not create hash table: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	atexit(cleanup);
	handle_options(argc, argv);
	init();
	log_info("Starting to parse\n");
	parse();
	log_info("Parsing completed, generating output\n");
	validate();
	output();
	log_info("Success, generated output\n");
	exit(EXIT_SUCCESS);
}
