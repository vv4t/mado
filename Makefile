.PHONY=default run

CFLAGS=-O3 -Wall
LDFLAGS=-lSDL2 -lSDL2_image -lm
ifeq ($(OS),Windows_NT)
    LDFLAGS+= -l:libglew32.dll.a -lopengl32
else
    LDFLAGS+= -lGL -lGLEW
endif
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
