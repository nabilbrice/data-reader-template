.PHONY: clean, test, test-run

CC ?= clang
CFLAGS := -Wextra -Wall -Wconversion -Wcast-align -Wstrict-prototypes

clean:
	rm ./test

test: main.c driver_table_reader.c
	$(CC) $(CFLAGS) -o test main.c driver_table_reader.c

test-run: test
	./test
