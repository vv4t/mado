.PHONY=default run

CFLAGS=-O3
LDFLAGS=-lSDL2 -lSDL2_image -lm -lGL -lGLEW
SRC=$(wildcard src/*.c) $(wildcard src/*/*.c)
OBJ=$(patsubst src/%.c, bin/%.o, $(SRC))

defualt: mado run

mado: $(OBJ)
	gcc $(CFLAGS) $(LDFLAGS) $^ -o $@

bin/%.o: src/%.c | bin/common/ bin/game/ bin/renderer/
	gcc $(CFLAGS) -c -o $@ $^

bin/common/: | bin
	mkdir -p $@

bin/game/: | bin
	mkdir -p $@

bin/renderer/: | bin
	mkdir -p $@

bin:
	mkdir -p $@

run:
	./mado
