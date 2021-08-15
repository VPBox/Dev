#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sepol/policydb/policydb.h>
#include <sepol/policydb/services.h>
#include <sepol/policydb/expand.h>

#define EQUALS 0
#define NOT 1
#define ANY 2

void usage(char *arg0) {
	fprintf(stderr, "%s -s <source> -t <target> -c <class> -p <perm> -P <policy file>\n", arg0);
	exit(1);
}

void *cmalloc(size_t s) {
	void *t = malloc(s);
	if (t == NULL) {
		fprintf(stderr, "Out of memory\n");
		exit(1);
	}
	return t;
}

int parse_ops(char **arg) {
	switch (*arg[0]) {
		case '-':
			*arg = *arg + 1;
			return NOT;
		case '*':
			return ANY;
		default:
			return EQUALS;
	}
}

int check(int op, uint16_t arg1, uint16_t arg2) {
	switch (op) {
		case EQUALS:
			return arg1 == arg2;
		case NOT:
			return arg1 != arg2;
		case ANY:
			return 1;
		default:
			fprintf(stderr, "Bad op while checking!");
			return 2;
	}
}

int check_perm(avtab_ptr_t current, perm_datum_t *perm) {
	uint16_t perm_bitmask = 1U << (perm->s.value - 1);
	return (current->datum.data & perm_bitmask) != 0;
}


int expand_and_check(int s_op, uint32_t source_type,
		     int t_op, uint32_t target_type,
		     int c_op, uint32_t target_class,
		     perm_datum_t *perm, policydb_t *policy, avtab_t *avtab) {
	avtab_t exp_avtab;
	avtab_ptr_t cur;
	unsigned int i;
	int match;

	if (avtab_init(&exp_avtab)) {
		fputs("out of memory\n", stderr);
		return -1;
	}

	if (expand_avtab(policy, avtab, &exp_avtab)) {
		fputs("out of memory\n", stderr);
		avtab_destroy(&exp_avtab);
		return -1;
	}

	for (i = 0; i < exp_avtab.nslot; i++) {
		for (cur = exp_avtab.htable[i]; cur; cur = cur->next) {
			match = 1;
			match &= check(s_op, source_type, cur->key.source_type);
			match &= check(t_op, target_type, cur->key.target_type);
			match &= check(c_op, target_class, cur->key.target_class);
			match &= check_perm(cur, perm);
			if (match) {
				avtab_destroy(&exp_avtab);
				return 1;
			}
		}
	}

	avtab_destroy(&exp_avtab);
	return 0;
}

/*
 * Checks to see if a rule matching the given arguments already exists.
 *
 * The format for the arguments is as follows:
 *
 * - A bare string is treated as a literal and will be matched by equality.
 * - A string starting with "-" will be matched by inequality.
 * - A string starting with "*" will be treated as a wildcard.
 *
 * The return codes for this function are as follows:
 *
 * - 0 indicates a successful return without a match
 * - 1 indicates a successful return with a match
 * - -1 indicates an error
 */
int check_rule(char *s, char *t, char *c, char *p, policydb_t *policy) {
	type_datum_t *src = NULL;
	type_datum_t *tgt = NULL;
	class_datum_t *cls = NULL;
	perm_datum_t *perm = NULL;
	int s_op = parse_ops(&s);
	int t_op = parse_ops(&t);
	int c_op = parse_ops(&c);
	int p_op = parse_ops(&p);
	avtab_key_t key;
	int match;

	key.source_type = key.target_type = key.target_class = 0;

	if (s_op != ANY) {
		src = hashtab_search(policy->p_types.table, s);
		if (src == NULL) {
			fprintf(stderr, "source type %s does not exist\n", s);
			return -1;
		}
	}
	if (t_op != ANY) {
		tgt = hashtab_search(policy->p_types.table, t);
		if (tgt == NULL) {
			fprintf(stderr, "target type %s does not exist\n", t);
			return -1;
		}
	}
	if (c_op != ANY) {
		cls = hashtab_search(policy->p_classes.table, c);
		if (cls == NULL) {
			fprintf(stderr, "class %s does not exist\n", c);
			return -1;
		}
	}
	if (p_op != ANY) {
		perm = hashtab_search(cls->permissions.table, p);
		if (perm == NULL) {
			if (cls->comdatum == NULL) {
				fprintf(stderr, "perm %s does not exist in class %s\n", p, c);
				return -1;
			}
			perm = hashtab_search(cls->comdatum->permissions.table, p);
			if (perm == NULL) {
				fprintf(stderr, "perm %s does not exist in class %s\n", p, c);
				return -1;
			}
		}
	}

	if (s_op != ANY)
		key.source_type = src->s.value;
	if (t_op != ANY)
		key.target_type = tgt->s.value;
	if (c_op != ANY)
		key.target_class = cls->s.value;

	/* Check unconditional rules after attribute expansion. */
	match = expand_and_check(s_op, key.source_type,
				 t_op, key.target_type,
				 c_op, key.target_class,
				 perm, policy, &policy->te_avtab);
	if (match)
		return match;

	/* Check conditional rules after attribute expansion. */
	return expand_and_check(s_op, key.source_type,
				t_op, key.target_type,
				c_op, key.target_class,
				perm, policy, &policy->te_cond_avtab);
}

int load_policy(char *filename, policydb_t *policydb, struct policy_file *pf) {
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


int main(int argc, char **argv)
{
	char *policy = NULL, *source = NULL, *target = NULL, *class = NULL, *perm = NULL;
	policydb_t policydb;
	struct policy_file pf;
	sidtab_t sidtab;
	char ch;
	int match = 1;

	struct option long_options[] = {
			{"source", required_argument, NULL, 's'},
			{"target", required_argument, NULL, 't'},
			{"class", required_argument, NULL, 'c'},
			{"perm", required_argument, NULL, 'p'},
			{"policy", required_argument, NULL, 'P'},
			{NULL, 0, NULL, 0}
	};

	while ((ch = getopt_long(argc, argv, "s:t:c:p:P:", long_options, NULL)) != -1) {
		switch (ch) {
			case 's':
				source = optarg;
				break;
			case 't':
				target = optarg;
				break;
			case 'c':
				class = optarg;
				break;
			case 'p':
				perm = optarg;
				break;
			case 'P':
				policy = optarg;
				break;
			default:
				usage(argv[0]);
		}
	}

	if (!source || !target || !class || !perm || !policy)
		usage(argv[0]);

	sepol_set_policydb(&policydb);
	sepol_set_sidtab(&sidtab);

	if (load_policy(policy, &policydb, &pf))
		goto out;

	match = check_rule(source, target, class, perm, &policydb);
	if (match < 0) {
		fprintf(stderr, "Error checking rules!\n");
		goto out;
	} else if (match > 0) {
		printf("Match found!\n");
		goto out;
	}

	match = 0;

out:
	policydb_destroy(&policydb);
	return match;
}
