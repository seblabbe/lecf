CFLAGS=-Wall
ALGORITHMS_SRC=$(wildcard algorithms/*.c)
ALGORITHMS=$(ALGORITHMS_SRC:algorithms%.c=.%)

all: $(ALGORITHMS)

%: algorithms/%.c lecf.c
	@echo $<
	gcc -D ALGO_FILE=$< $(CFLAGS) -o $@ -lm lecf.c

