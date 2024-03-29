#include <time.h>
#include <stdint.h>
#include <memory.h>
#include "utils.h"
#include "log.h"

#define CHECK_PTR(ptr,retval)                             \
if (!ptr) {                                               \
    LOG(DEBUG_ERR, stderr, "pointer: "#ptr" is NULL!\n")  \
    return retval;                                        \
}

static unsigned char *generate_data(size_t size)
{
    int sizeof_int = sizeof(int);
    if (size % sizeof_int != 0) {
        LOG_PARAMS(DEBUG_ERR, stderr, "size must be divisible by %d\n", sizeof_int);
        return NULL;
    }

    unsigned char* data = (unsigned char*)malloc(size);
    CHECK_PTR(data, NULL);

    int num_gen = size / sizeof_int;
    int content;

    while (num_gen > 0) {
        num_gen--;
        content = rand();
        memcpy(data, &content, sizeof_int);
    }

    return data;
}

static int write_data_to_file(FILE *fp, unsigned char *data, size_t size)
{
    LOG(DEBUG_DBG, stdout, "Start writing data to the file...\n");

    CHECK_PTR(fp, 0);
    CHECK_PTR(data, 0);

    size_t nbytes_write = fwrite(data, sizeof(unsigned char), size, fp);

    if (nbytes_write != size) {
        LOG_PARAMS(DEBUG_WARN, stderr, "%8ld bytes should be written to the file, but only %8ld bytes are being written!\n", nbytes_write, size);
        return 0;
    }

    LOG_PARAMS(DEBUG_DBG, stdout, "%8ld Bytes are written\n", nbytes_write);

    return 1;
}

static inline int file_is_full(long long int nbytes_write, long long int size)
{
    return (size - nbytes_write) > 0? 0 : 1; 
}

static inline size_t generate_random_data_size(void)
{
    int size = MIN_CHUNK_SIZE + (rand() % (MAX_CHUNK_SIZE-MIN_CHUNK_SIZE));
    size += (4-(size % 4));
    if (size > MAX_CHUNK_SIZE)
        size = MAX_CHUNK_SIZE;
    return (size_t)size;
}

int try_open(FILE **fp, const char *fname, const char *mode)
{
    LOG(DEBUG_INFO, stdout, "Try opening the file...\n");

    FILE *tmp = NULL;
    if ((tmp = fopen(fname, mode)) != NULL) {
        *fp = tmp;
    }
    else {
        LOG(DEBUG_FATAL, stderr, "Failed to open the file!\n");
        return 0;
    }

    LOG(DEBUG_INFO, stdout, "Opening the file successfully\n");

    return 1;
}

int generate_file(FILE *fp, const char *fname, long long int size, gen_option_t option)
{
    LOG(DEBUG_INFO, stdout, "Start generating file...\n");

    CHECK_PTR(fp, 0);
    CHECK_PTR(fname, 0);

    srand(time(NULL));

    unsigned char *data = NULL;
    long long int nbytes_write = (long long int)0;
    long long int bytes_to_write = DEFAULT_CHUNK_SIZE;

    if (!option.random_content && !option.random_block_size)
        data = generate_data(DEFAULT_CHUNK_SIZE);
    else if (!option.random_content && option.random_block_size) {
        size_t random_size = generate_random_data_size();
        data = generate_data(random_size);   
        bytes_to_write = random_size;
    }

    while (!file_is_full(nbytes_write, size)) {
        if (option.random_block_size && option.random_content) {
            if (data)
                free(data);
            size_t random_size = generate_random_data_size();
            data = generate_data(random_size);
            bytes_to_write = random_size;
        }
        if (write_data_to_file(fp, data, bytes_to_write)!=1) {
            LOG(DEBUG_ERR, stderr, "Failed to write chunk data to the file!\n");
            return 0;
        }
        else {
            nbytes_write += bytes_to_write;
            LOG_PARAMS(DEBUG_DBG, stderr, "%12lld bytes are written to the file\n", nbytes_write);
        }
    }

    if (data) {
        LOG(DEBUG_DBG, stdout, "Freeing the generated data...\n");
        free(data);
        data = NULL;
    }

    LOG(DEBUG_INFO, stdout, "Generating a file successfully!\n");

    return 1;
}