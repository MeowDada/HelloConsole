.PHONY: all clean

CC := gcc
HELLO_CONSOLE_SRC := hello_console.c
HELLO_CONSOLE_LONG_SRC := hello_console_getopt_long.c
HELLO_CONSOLE_TARGET := $(patsubst %.c,%.o, $(HELLO_CONSOLE_SRC))
HELLO_CONSOLE_LONG_TARGET := $(patsubst %.c,%.o, $(HELLO_CONSOLE_LONG_SRC))

all: mk_hello_console mk_hello_console_getopt_long

mk_hello_console:
	$(CC) -o $(HELLO_CONSOLE_TARGET) $(HELLO_CONSOLE_SRC)

mk_hello_console_getopt_long:
	$(CC) -o $(HELLO_CONSOLE_LONG_TARGET) $(HELLO_CONSOLE_LONG_SRC) utils.c

clean: clean_hello_console clean_hello_console_getopt_long

clean_hello_console:
	rm $(HELLO_CONSOLE_TARGET)

clean_hello_console_getopt_long:
	rm $(HELLO_CONSOLE_LONG_TARGET)