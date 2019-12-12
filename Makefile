CFLAGS = -g

.PHONY: all
all: logf

.PHONY: run
run: test

.PHONY: clean
clean:
	-rm logf *.yy.c *.tab.c *.o

logfile.tab.c logfile.tab.h: logfile.y journal.h
	bison -d logfile.y

lex.yy.c: logfile.l logfile.tab.h
	flex logfile.l

logf: lex.yy.o logfile.tab.o journal.o
	gcc $(CFLAGS) $^ -o logf

%.o: %.c
	gcc -c $(CFLAGS) $< -o $@

.PHONY: test
test: logf
	-./logf examples/example_1
