.PHONY=default run

CFLAGS=-O3
LDFLAGS=-lSDL2 -lSDL2_image -lm -lGL -lGLEW
SRC=$(wildcard src/*.c) $(wildcard src/*/*.c) $(wildcard src/*/*/*.c)
OBJ=$(patsubst src/%.c, bin/%.o, $(SRC))
SRC_H=$(wildcard src/*.h) $(wildcard src/*/*.h) $(wildcard src/*/*/*.h)

defualt: mado run

mado: $(OBJ)
	gcc $(CFLAGS) $(LDFLAGS) $^ -o $@

bin/%.o: src/%.c $(SRC_H) | bin/common/ bin/game/entity/ bin/game/component/ bin/game/system/ bin/game/ bin/renderer/
	gcc $(CFLAGS) -c -o $@ $<

bin/common/: | bin/
	mkdir -p $@

bin/game/entity/: | bin/game/
	mkdir -p $@

bin/game/component/: | bin/game/
	mkdir -p $@

bin/game/system/: | bin/game/
	mkdir -p $@

bin/game/: | bin
	mkdir -p $@

bin/renderer/: | bin/
	mkdir -p $@

bin/:
	mkdir -p $@

run:
	./mado
