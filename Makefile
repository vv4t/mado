.PHONY=default run build web

CFLAGS=-O3 -Wall
LDFLAGS=-lSDL2 -lSDL2_image -lm -lGL -lGLEW
INCLUDE=-I src/include
SRC=$(wildcard src/*/*.c)
OBJ=$(patsubst src/%.c, bin/%.o, $(SRC))
SRC_H=$(wildcard src/*/*.h)

ASSETS_PNG=$(wildcard assets/*/*.png)
ASSETS_MAP=$(wildcard assets/*/*.map)
ASSETS_FRAG=$(wildcard assets/*/*/*.frag)
ASSETS_VERT= $(wildcard assets/*/*/*.vert)
ASSETS_GLSL=$(wildcard assets/*/*/*.glsl)
ASSETS_SRC=$(ASSETS_PNG) $(ASSETS_MAP) $(ASSETS_VERT) $(ASSETS_FRAG) $(ASSETS_GLSL)

WEB_OUT=bin/web/index.html
WEB_CFLAGS=-s WASM=1 -O3
WEB_LDFLAGS=-s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_WEBGL2=1
WEB_SRC=src/*/*.c
WEB_ASSETS=$(patsubst %, --preload-file %, $(ASSETS_SRC)) --use-preload-plugins

default: mado run

mado: $(OBJ)
	gcc $(CFLAGS) $^ $(LDFLAGS) -o $@

bin/%.o: src/%.c $(SRC_H)
	mkdir -p bin
	mkdir -p "$(shell dirname "$@")"
	gcc $(INCLUDE) $(CFLAGS) -c -o $@ $<

build:
	gcc $(INCLUDE) $(CLAGS) $(LDFLAGS) $(SRC) -o mado

web: src/*/*.c src/*/*/*.h $(ASSETS_SRC)
	mkdir -p bin
	mkdir -p bin/web
	echo $(ASSETS_MAP)
	emcc $(INCLUDE) $(WEB_CFLAGS) $(WEB_LDFLAGS) $(WEB_ASSETS) $(WEB_SRC) -o $(WEB_OUT) 

run: mado
	./mado
