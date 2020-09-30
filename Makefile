all: build
build: src/elizaFlood.c src/elizaFlood-lib.c src/elizaFlood.h
	gcc -o bin/ElizaFlood src/elizaFlood.c src/elizaFlood-lib.c
	gcc -o bin/Gui src/gui.c src/elizaFlood-lib.c´ `pkg-config --libs --cflags gtk+-3.0`

