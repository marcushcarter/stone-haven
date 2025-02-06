compilet: windres buildt run

compile: windres build run

windres:
	windres src/resource.rc -O coff -o src/resource.res

build:
	gcc -mwindows -I include -L lib -o stone-haven src/main.c src/resource.res -lmingw32 -lSDL3 -lSDL3_image

buildt:
	gcc -I include -L lib -o stone-haven src/main.c src/resource.res -lmingw32 -lSDL3 -lSDL3_image

run:
	./stone-haven.exe
