/*
 * version_policy.c - Takes the given public platform policy, a private policy
 * and a version number to produced a combined "versioned" policy file.
 */
#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/stat.h>
#include <cil/android.h>
#include <cil/cil.h>
#include <cil/cil_write_ast.h>

void __attribute__ ((noreturn)) static usage(char *prog) {
	printf("Usage: %s [OPTION]...\n", prog);
	printf("\n");
	printf("Options:\n");
	printf("  -b, --base=<file>          (req'd) base policy for versioning.\n");
	printf("  -m, --mapping              generate cil version  mapping from base policy\n");
	printf("  -n, --number               (req'd) version number to use.\n");
	printf("  -o, --output=<file>        write cil policy to <file>\n");
	printf("  -t, --tgt_policy           policy to be versioned according to base policy\n");
	printf("  -h, --help                 display usage information\n");
	exit(1);
}

/*
 * read_cil_file - Initialize db and parse CIL input file.
 */
static int read_cil_file(struct cil_db **db, char *path) {
	int rc = SEPOL_ERR;
	FILE *file;
	struct stat filedata;
	uint32_t file_size;
	char *buff = NULL;

	cil_db_init(db);
	file = fopen(path, "re");
	if (!file) {
		fprintf(stderr, "Could not open file: %s\n", path);
		goto file_err;
	}
	rc = stat(path, &filedata);
	if (rc == -1) {
		fprintf(stderr, "Could not stat file: %s - %s\n", path, strerror(errno));
		goto err;
	}
	file_size = filedata.st_size;
	buff = malloc(file_size);
	if (buff == NULL) {
		fprintf(stderr, "OOM!\n");
		rc = SEPOL_ERR;
		goto err;
	}
	rc = fread(buff, file_size, 1, file);
	if (rc != 1) {
		fprintf(stderr, "Failure reading file: %s\n", path);
		rc = SEPOL_ERR;
		goto err;
	}
	fclose(file);
	file = NULL;

	/* creates parse_tree */
	rc = cil_add_file(*db, path, buff, file_size);
	if (rc != SEPOL_OK) {
		fprintf(stderr, "Failure adding %s to parse tree\n", path);
		goto err;
	}
	free(buff);

	return SEPOL_OK;
err:
	free(buff);
	fclose(file);
file_err:
	cil_db_destroy(db);
	return rc;
}

int main(int argc, char *argv[])
{
	int opt_char;
	int opt_index = 0;
	int rc = SEPOL_ERR;
	bool mapping = false;
	char *base = NULL;
	char *tgt_policy = NULL;
	char *num = NULL;
	char *dot;
	char *output = NULL;
	struct cil_db *base_db = NULL;
	struct cil_db *out_db = NULL;

	static struct option long_opts[] = {
		{"help", no_argument, 0, 'h'},
		{"base", required_argument, 0, 'b'},
		{"mapping", no_argument, 0, 'm'},
		{"number", required_argument, 0, 'n'},
		{"output", required_argument, 0, 'o'},
		{"tgt_policy", required_argument, 0, 't'},
		{0, 0, 0, 0}
	};

	while (1) {
		opt_char = getopt_long(argc, argv, "b:mn:o:t:h", long_opts, &opt_index);
		if (opt_char == -1) {
			break;
		}
		switch (opt_char) {
		case 'b':
			base = strdup(optarg);
			break;
		case 'm':
			mapping = true;
			break;
		case 'n':
			num = strdup(optarg);
			break;
		case 'o':
			output = strdup(optarg);
			break;
		case 't':
			tgt_policy = strdup(optarg);
			break;
		case 'h':
			usage(argv[0]);
		default:
			fprintf(stderr, "Unsupported option: %s\n", optarg);
			usage(argv[0]);
		}
	}
	if (optind < argc) {
		fprintf(stderr, "Unknown arguments supplied\n");
		usage(argv[0]);
	}
	if (num == NULL || base == NULL || (mapping == false && tgt_policy == NULL)) {
		fprintf(stderr, "Please specify required arguments\n");
		usage(argv[0]);
	}

	/* policy language doesn't like '.', so replace them with '_' in mapping version */
	dot = num;
	while ((dot = strchr(dot, '.')) != NULL) {
		*dot = '_';
		++dot;
	}

	if (mapping && tgt_policy) {
		fprintf(stderr, "Please select only one mode between --mapping and --tgt_policy\n");
		usage(argv[0]);
	}

	/* gimme only the important details */
	cil_set_log_level(CIL_WARN);

	/* read platform policy */
	rc = read_cil_file(&base_db, base);
	if (rc != SEPOL_OK) {
		goto exit;
	}

	if (mapping) {
		rc = cil_android_attrib_mapping(&out_db, base_db, num);
		if (rc != SEPOL_OK)
			goto exit;
	} else {
		/* read target policy, ready for manipulation */
		rc = read_cil_file(&out_db, tgt_policy);
		if (rc != SEPOL_OK) {
			goto exit;
		}
		/* attributize the target policy */
		rc = cil_android_attributize(out_db, base_db, num);
		if (rc != SEPOL_OK) {
			goto exit;
		}
	}
	rc = cil_write_ast(out_db, output);
	if (rc != SEPOL_OK) {
		goto exit;
	}

exit:
	free(base);
	free(tgt_policy);
	free(num);
	free(output);
	cil_db_destroy(&base_db);
	cil_db_destroy(&out_db);
	return rc;
}
