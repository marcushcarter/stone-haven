compilet: buildt run

compile: build run

# Build with resource file
windres:
	windres resource.rc -o resource.res

build:
	gcc -mwindows -I include -L lib -o stone-haven src/main.c -lmingw32 -lSDL3 -lSDL3_image

# Build without resource file (fallback)
buildt:
	gcc -I include -L lib -o stone-haven src/main.c -lmingw32 -lSDL3 -lSDL3_image

# Run the executable
run:
	./stone-haven.exe
