
CFLAGS += -g


# https://tech.davis-hansson.com/p/make/
SHELL := bash
.ONESHELL:
.SHELLFLAGS := -eu -o pipefail -c
.DELETE_ON_ERROR:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules



.PHONY: all run clean test

.DEFAULT: all


all: logf

clean:
	-rm logf *.yy.c *.tab.c *.o

logfile.tab.c logfile.tab.h: logfile.y journal.h
	bison -d logfile.y

lex.yy.c: logfile.l logfile.tab.h
	flex logfile.l

logf: lex.yy.o logfile.tab.o journal.o helpers.o
	gcc $(CFLAGS) $^ -o logf

main: main.o helpers.o
	gcc $(CFLAGS) $^ -o main

%.o: %.c
	gcc -c $(CFLAGS) $< -o $@

test: logf
	-./logf examples/example_1

run: main
	-./main
