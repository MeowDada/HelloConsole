#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int continueous_mode = 0;

static void enable_command_mode(void)
{
    continueous_mode = 1;
}

static void guess_lucky_number(int guess)
{
    srand(time(NULL));
    int lucky_number = (rand() % 3) + 1;
    if (guess==lucky_number)
        fprintf(stdout, "BINGO!!!\n");
    else
        fprintf(stdout, "Nice tried!!!\n");
}

static void roll_dice(int num_rolls)
{
    srand(time(NULL));
    while (num_rolls--) {
        int value = (rand() % 6) + 1;
        fprintf(stdout, "You roll to %d !!!\n", value);
    }
}

static void show_program_name(const char *program_name)
{
    fprintf(stdout, "%s\n", program_name);
}

static void say_hello(void)
{
    fprintf(stdout, "Hello World!\n");
}

static void usage(const char *program_name)
{
    fprintf(stderr, "Usage: [-r<num_rolls>] [-g<guess_number>]\n");
    fprintf(stderr, "Where:\n");
    fprintf(stderr, "   -g - Guess a lucky number (1-3)\n");
    fprintf(stderr, "   -c - Enable continueous command mode\n");
    fprintf(stderr, "   -r - Roll a dice and show its value\n");
    fprintf(stderr, "   -S - Show the program name\n");
    fprintf(stderr, "   -s - Say Hello to this world\n");
    fprintf(stderr, "   -q - Quit\n");
    fprintf(stderr, "   -? - This help text\n");
}

int main(int argc, char **argv) 
{
    const char *program_name;
    int opt;

    program_name = argv[0];

    /* 
     * Parse startup command by using build-in function getopt(...)
     */
    while ((opt = getopt(argc, argv, "g:cr::Ssq")) != -1) {
        switch (opt) {
            case 'g':
                guess_lucky_number(atoi(optarg));
                break;
            case 'c':
                enable_command_mode();
                break;
            case 'r':
                if (optarg) {
                    fprintf(stderr, "option = %s\n", optarg);
                    int num_rolls = atoi(optarg);
                    fprintf(stderr, "Roll dice for %d times !!!\n", num_rolls);
                    roll_dice(num_rolls);
                }
                else 
                    roll_dice(1);
                break;
            case 'S':
                show_program_name(program_name);
                break;
            case 's':
                say_hello();
                break;
            case 'q':
                exit(0);
                break;
            case 'h':
            case '?':
            default:
                usage(program_name);
                break;
        }
    }

    /*
     * Handle those options which are not being handle by getopt()
     */
    argc -= optind;
    argv += optind;

    if (continueous_mode) {
        
    }

    return 0;
}