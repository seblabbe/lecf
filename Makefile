CFLAGS=-Wall
ALGORITHMS_RAW=$(wildcard src/algorithms/*.c)
ALGORITHMS=$(ALGORITHMS_RAW:src/algorithms/%.c=%)

all: $(ALGORITHMS)
	./scripts/make_main_include_file $(ALGORITHMS)
	echo "#include \"mcf_algorithms.h\"" > src/mcf_algorithms.c
	cat build/*.c >> src/mcf_algorithms.c
	python setup.py build_ext --inplace

clean:
	if [ -d build ]; then rm -rf build/; fi
	if [ -f src/mcf_algorithms.c ]; then rm src/mcf_algorithms.c; fi
	if [ -f src/algorithms_array.c ]; then rm src/algorithms_array.c; fi

%: src/algorithms/%.c src/lecf.c
	@if [ ! -d build ]; then mkdir build; fi
	gcc -E -D ALGO_FILE=algorithms/$@.c src/lecf.c | grep -v "^#" > build/$@.c


