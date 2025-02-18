compilet: buildt run

compile: build run

build:
	windres src/resource.rc -O coff -o src/resource.res
	gcc -mwindows -I include -L lib -o stone-haven src/main.c src/resource.res -lmingw32 -lSDL3 -lSDL3_image

buildt:
	windres src/resource.rc -O coff -o src/resource.res
	gcc -g -I include -L lib -o stone-haven src/main.c src/resource.res -lmingw32 -lSDL3 -lSDL3_image

run:
	./stone-haven.exe
