#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "log.h"
#include "utils.h"

#define DEFAULT_OUTPUT_FILE_SIZE (65536LL)

static struct option cmds[] = {
    {"file",    required_argument,  NULL,   'f'},
    {"size",    required_argument,  NULL,   's'},
    {"debug",   optional_argument,  NULL,   'd'},
    {"random",  no_argument,        NULL,   'r'},
    {"var",     no_argument,        NULL,   'v'},
    {"help",    no_argument,        NULL,   'h'}
};

const char *cmd_format = "f:s::d::rvh";

int debug_level = DEFAULT_DEBUG_LEVEL;

static gen_option_t gen_opt = {
    .random_block_size = 0,
    .random_content = 0
};
static long long output_file_size = DEFAULT_OUTPUT_FILE_SIZE;
char *output_file_name = NULL;
const char *mode = "wb+";

static void usage(const char *program_name)
{
    fprintf(stdout, "\n");
    fprintf(stdout, "Usage: %s -f <filename> [OPTIONS]...\n", program_name);
    fprintf(stdout, "Generate a file with given <filename> and size 64KB at least\n");
    fprintf(stdout, "\n");
    fprintf(stdout, "[MUST]:\n");
    fprintf(stdout, "   -f, --file <filename>    specify the output file name with <filename>\n");
    fprintf(stdout, "[OPTIONS]:\n");
    fprintf(stdout, "   -s, --size <size>        generate the file with given <size>\n");
    fprintf(stdout, "   -d, --debug[=level]      specify the debug level, if not given <level>,\n");
    fprintf(stdout, "                            debug level will be set to DEBUG_FATAL)\n");
    fprintf(stdout, "   -r, --random             enable random generate content mode\n");
    fprintf(stdout, "   -v, --variable           enable random block size mode\n");
    fprintf(stdout, "   -h, --help               show this help text\n");
    fprintf(stdout, "\n");
    fprintf(stdout, "Note:\n");
    fprintf(stdout, "Each file is generated with many basic units called \"blocks\"\n");
    fprintf(stdout, "And each block could be at different sizes but in\n");
    fprintf(stdout, "range [%8d,%8d]. By default, all the block size will be %d.\n", DEFAULT_CHUNK_SIZE);
}

int main(int argc, char **argv)
{
    int opt;
    const char *program_name = argv[0];
    FILE *fp = NULL;

    while ((opt = getopt_long(argc, argv, cmd_format, cmds, NULL)) != -1) {
        switch (opt)
        {
            case 'f':
                if (!optarg) {
                    LOG(DEBUG_FATAL, stderr, "Must specify the output file name with -f <filename>!\n");
                    goto end;
                }
                strcpy(output_file_name, optarg);
                break;
            case 's':
                if (!optarg) {
                    LOG(DEBUG_FATAL, stderr, "Must specify the output file size with -s <size> (KBs)!\n");
                    goto end;
                }
                output_file_size = atoll(optarg);
                LOG_PARAMS(DEBUG_WARN, stderr, "Debug file size = %lld\n", output_file_size);
                break;
            case 'd':
                if (!optarg) {
                    debug_level = DEBUG_FATAL;
                    LOG_PARAMS(DEBUG_INFO, stdout, "Debug level = %d\n", debug_level);
                }
                else {
                    debug_level = atoi(optarg);
                    LOG_PARAMS(DEBUG_INFO, stdout, "Debug level = %d\n", debug_level);
                }
                break;
            case 'r':
                gen_opt.random_content = 1;
                break;
            case 'v':
                gen_opt.random_block_size = 1;
                break;
            case 'h':
            case '?':
            default:
                usage(program_name);
                goto end;
                break;
        }
    }

    if (!try_open(&fp, output_file_name, mode)) {
        LOG(DEBUG_FATAL, stderr, "Failed to open the file!\n");
        goto end;
    }

    if (!generate_file(fp, output_file_name, output_file_size, gen_opt)) {
        LOG(DEBUG_FATAL, stderr, "Failed to generate the file!\n");
        goto end;
    }

end:
    fclose(fp);
    return EXIT_SUCCESS;
}