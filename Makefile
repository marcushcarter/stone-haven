compilet: buildt run

compile: build run

build:
	gcc -mwindows -I include -L lib -o stone-haven src/main.c -lmingw32 -lSDL3 -lSDL3_image

buildt:
	gcc -I include -L lib -o stone-haven src/main.c -lmingw32 -lSDL3 -lSDL3_image

run:
	./stone-haven.exe
