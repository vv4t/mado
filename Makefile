.PHONY=default run

CFLAGS=-O3 -Wall
LDFLAGS=-lSDL2 -lSDL2_image -lm -l:libglew32.dll.a -lopengl32
INCLUDE=-I src/include
SRC=$(wildcard src/*/*.c)
OBJ=$(patsubst src/%.c, bin/%.o, $(SRC))
SRC_H=$(wildcard src/*/*.h)

default: mado run

mado: $(OBJ)
	gcc $(CFLAGS) $^ $(LDFLAGS) -o $@

bin/%.o: src/%.c $(SRC_H)
	mkdir -p bin
	mkdir -p "$(shell dirname "$@")"
	gcc $(INCLUDE) $(CFLAGS) -c -o $@ $<

run: mado
	./mado
