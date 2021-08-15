#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sched.h>
#include <sys/resource.h>
#include <ctype.h>
#define USEC_PER_SEC 1000000ULL
#define MAX_COUNT 1000000000ULL
#define NUM_INSTS_GARBAGE 18

// Contains information about benchmark options.
typedef struct {
    int cpu_to_lock;
    int locked_freq;
} command_data_t;

void usage() {
    printf("--------------------------------------------------------------------------------\n");
    printf("Usage:");
    printf("	crypto [--cpu_to_lock CPU] [--locked_freq FREQ_IN_KHZ]\n\n");
    printf("!!!!!!Lock the desired core to a desired frequency before invoking this benchmark.\n");
    printf(
          "Hint: Set scaling_max_freq=scaling_min_freq=FREQ_IN_KHZ. FREQ_IN_KHZ "
          "can be obtained from scaling_available_freq\n");
    printf("--------------------------------------------------------------------------------\n");
}

int processOptions(int argc, char **argv, command_data_t *cmd_data) {
    // Initialize the command_flags.
    cmd_data->cpu_to_lock = 0;
    cmd_data->locked_freq = 1;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            int *save_value = NULL;
            if (strcmp(argv[i], "--cpu_to_lock") == 0) {
                save_value = &cmd_data->cpu_to_lock;
	    } else if (strcmp(argv[i], "--locked_freq") == 0) {
                save_value = &cmd_data->locked_freq;
            } else {
                printf("Unknown option %s\n", argv[i]);
                return -1;
            }
            if (save_value) {
                // Checking both characters without a strlen() call should be
                // safe since as long as the argument exists, one character will
                // be present (\0). And if the first character is '-', then
                // there will always be a second character (\0 again).
                if (i == argc - 1 ||
                    (argv[i + 1][0] == '-' && !isdigit(argv[i + 1][1]))) {
                    printf("The option %s requires one argument.\n", argv[i]);
                    return -1;
                }
                *save_value = (int)strtol(argv[++i], NULL, 0);
            }
	}
    }
    return 0;
}
/* Performs encryption on garbage values. In Cortex-A57 r0p1 and later
 * revisions, pairs of dependent AESE/AESMC and AESD/AESIMC instructions are
 * higher performance when adjacent, and in the described order below. */
void garbage_encrypt() {
    __asm__ __volatile__(
	"aese  v0.16b, v4.16b ;"
        "aesmc	v0.16b, v0.16b ;"
        "aese  v1.16b, v4.16b ;"
        "aesmc	v1.16b, v1.16b ;"
        "aese  v2.16b, v4.16b ;"
        "aesmc	v2.16b, v2.16b ;"
        "aese  v0.16b, v5.16b ;"
        "aesmc	v0.16b, v0.16b ;"
        "aese  v1.16b, v5.16b ;"
        "aesmc	v1.16b, v1.16b ;"
        "aese  v2.16b, v5.16b ;"
        "aesmc	v2.16b, v2.16b ;"
        "aese  v0.16b, v6.16b ;"
        "aesmc	v0.16b, v0.16b ;"
        "aese  v1.16b, v6.16b ;"
        "aesmc	v1.16b, v1.16b ;"
        "aese  v2.16b, v6.16b ;"
        "aesmc	v2.16b, v2.16b ;");
}

void garbage_decrypt() {
    __asm__ __volatile__(
	"aesd  v0.16b, v4.16b ;"
        "aesimc	v0.16b, v0.16b ;"
        "aesd  v1.16b, v4.16b ;"
        "aesimc	v1.16b, v1.16b ;"
        "aesd  v2.16b, v4.16b ;"
        "aesimc	v2.16b, v2.16b ;"
        "aesd  v0.16b, v5.16b ;"
        "aesimc	v0.16b, v0.16b ;"
        "aesd  v1.16b, v5.16b ;"
        "aesimc	v1.16b, v1.16b ;"
        "aesd  v2.16b, v5.16b ;"
        "aesimc	v2.16b, v2.16b ;"
        "aesd  v0.16b, v6.16b ;"
        "aesimc	v0.16b, v0.16b ;"
        "aesd  v1.16b, v6.16b ;"
        "aesimc	v1.16b, v1.16b ;"
        "aesd  v2.16b, v6.16b ;"
        "aesimc	v2.16b, v2.16b ;");
}


int main(int argc, char **argv) {
    usage();
    command_data_t cmd_data;

    if(processOptions(argc, argv, &cmd_data) == -1) {
        usage();
        return -1;
    }
    unsigned long long count = 0;
    struct timeval begin_time, end_time, elapsed_time;
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(cmd_data.cpu_to_lock, &cpuset);
    if (sched_setaffinity(0, sizeof(cpuset), &cpuset) != 0) {
	perror("sched_setaffinity failed");
	return 1;
    }
    gettimeofday(&begin_time, NULL);
    while (count < MAX_COUNT) {
      garbage_encrypt();
      count++;
    }
    gettimeofday(&end_time, NULL);
    timersub(&end_time, &begin_time, &elapsed_time);
    fprintf(stderr, "encrypt: %llu us\n",
            elapsed_time.tv_sec * USEC_PER_SEC + elapsed_time.tv_usec);
    fprintf(stderr, "encrypt instructions: %llu\n",
            MAX_COUNT * NUM_INSTS_GARBAGE);
    fprintf(stderr, "encrypt instructions per second: %f\n",
            (float)(MAX_COUNT * NUM_INSTS_GARBAGE * USEC_PER_SEC) /
                (elapsed_time.tv_sec * USEC_PER_SEC + elapsed_time.tv_usec));
    if (cmd_data.locked_freq != 0) {
	fprintf(stderr, "encrypt instructions per cycle: %f\n",
		(float)(MAX_COUNT * NUM_INSTS_GARBAGE * USEC_PER_SEC) /
		((elapsed_time.tv_sec * USEC_PER_SEC + elapsed_time.tv_usec) *
		 1000 * cmd_data.locked_freq));
    }
    printf("--------------------------------------------------------------------------------\n");

    count = 0;
    gettimeofday(&begin_time, NULL);
    while (count < MAX_COUNT) {
      garbage_decrypt();
      count++;
    }
    gettimeofday(&end_time, NULL);
    timersub(&end_time, &begin_time, &elapsed_time);
    fprintf(stderr, "decrypt: %llu us\n",
            elapsed_time.tv_sec * USEC_PER_SEC + elapsed_time.tv_usec);
    fprintf(stderr, "decrypt instructions: %llu\n",
            MAX_COUNT * NUM_INSTS_GARBAGE);
    fprintf(stderr, "decrypt instructions per second: %f\n",
            (float)(MAX_COUNT * NUM_INSTS_GARBAGE * USEC_PER_SEC) /
                (elapsed_time.tv_sec * USEC_PER_SEC + elapsed_time.tv_usec));
    if (cmd_data.locked_freq != 0) {
	fprintf(stderr, "decrypt instructions per cycle: %f\n",
		(float)(MAX_COUNT * NUM_INSTS_GARBAGE * USEC_PER_SEC) /
		((elapsed_time.tv_sec * USEC_PER_SEC + elapsed_time.tv_usec) *
		 1000 * cmd_data.locked_freq));
    }
    return 0;
}
