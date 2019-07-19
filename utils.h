#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_CHUNK_SIZE (65536)  /* 1024 * 64B = 64KB   */
#define MIN_CHUNK_SIZE     (32768)  /* 1024 * 32B = 32KB   */
#define MAX_CHUNK_SIZE     (131072) /* 1024 * 128B = 128KB */

typedef struct gen_option {
    int random_block_size;
    int random_content;
} gen_option_t;

int try_open(FILE **fp, const char *fname, const char *mode);
int generate_file(FILE *fp, const char *fname, long long int size, gen_option_t option);

#endif /* UTILS_H */