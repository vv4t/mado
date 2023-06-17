.PHONY=default run

CFLAGS=-O3
LDFLAGS=-lSDL2 -lSDL2_image -lm -lGL -lGLEW
SRC=src/*.c src/*/*.c

defualt: mado run

mado: $(SRC) src/*/*.h
	gcc $(CFLAGS) $(SRC) $(LDFLAGS) -o $@

bin:
	mkdir -p bin

run:
	./mado
