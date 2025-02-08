
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <windows.h>

#include "struct.c"

#include "textures.c"

clock_t previous_time = 0;
float dt;

int blockheld = 0;

float get_delta_time() {
	clock_t current_time = clock();
	float delta_time = (float)(current_time - previous_time) / CLOCKS_PER_SEC;
	previous_time = current_time;
	return delta_time;
}

void control_fps() {
	clock_t frame_start_time = clock();
	while ((float)(clock() - frame_start_time) / CLOCKS_PER_SEC < (1.0 / set.target_fps)) {}    
}

float distance2d(float x1, float y1, float x2, float y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

float randfloat(float min, float max) {
    float random = (float)rand() / (float)RAND_MAX;
    return min + random * (max - min);
}

int randint(int min, int max) {
    return min + rand() % (max - min + 1);
}

float* floatarr(int num, ...) {
	va_list args;
	float* combined_array = (float*)malloc(num * sizeof(float)); // Allocate memory for the integers
	int count = 0;
	va_start(args, num);
	for (int i = 0; i < num; i++) combined_array[count++] = va_arg(args, double);
	va_end(args);

	return combined_array;
}

void text_rect(SDL_Renderer* renderer, float position[4], float frame[4], SDL_Texture* texture, bool isframe) {
  	if (isframe) {
		SDL_FRect rposition = {position[0], position[1], position[2], position[3]};
		SDL_FRect rframe = {frame[0], frame[1], frame[2], frame[3]};
		SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_LINEAR);
		SDL_RenderTexture(renderer, texture, &rframe, &rposition);
  	} else {
		SDL_FRect rposition = {position[0], position[1], position[2], position[3]};
		// SDL_FRect rframe = {frame[0], frame[1], frame[2], frame[3]};
		SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_LINEAR);
		SDL_RenderTexture(renderer, texture, NULL, &rposition);
  	}
}

void draw_rect(SDL_Renderer* renderer, float position[4], int color, int transparency, bool fill) {
  	SDL_SetRenderDrawColor(renderer, colors[color][0], colors[color][1], colors[color][2], transparency);
  	SDL_FRect rect = {position[0], position[1], position[2], position[3]};
	if (fill) {
  		SDL_RenderFillRect(renderer, &rect);
	} else {
		SDL_RenderRect(renderer, &rect);
	}
}

#include "automata.c"
#include "world.c"
#include "particle.c"
#include "collision.c"
#include "player.c"
#include "loadsave.c"
#include "ui.c"

// ----------------------------------------------------------------------------------------------------

void editor_controls(bool active) {
	if (active) {
		// if (key.righta) { camera.targetx+=500*dt; }
		// if (key.lefta) { camera.targetx-=500*dt; }
		// if (key.upa) { camera.targety-=500*dt; }
		// if (key.downa) { camera.targety+=500*dt; }

		if (key.n1) camera.freecam=true;
		if (key.n2) camera.freecam=false;

		if(key.ctrl && key.s) save_world("gamesaves/world.save");
		if (key.ctrl && key.y) load_world("gamesaves/world.save"); 
		if (key.ctrl && key.z) generate_world();
	}

  	if (camera.freecam) {
		if (key.righta) camera.targetx+=1000*dt;
		if (key.lefta) camera.targetx-=1000*dt;
		if (key.upa) camera.targety-=1000*dt;
		if (key.downa) camera.targety+=1000*dt;
	} else {
		camera.targetx = miner.x;
		camera.targety = miner.y;
	}
}

void update() {

	if (dt > 0.3) return; // if time in between frames is too much no motion will happen;

	update_keystates(true);
  	editor_controls(true);
	update_player(true);
	update_blocks(true);
	update_particles(true);

	// if (key.o) { create_particle(1, P_IMPACT, miner.x, miner.y-25, 200-random_float()*400, -300, 1, 1); }
	// if (key.p) { create_particle(50, P_IMPACT, miner.x, miner.y-25, 200-random_float()*400, -300, 1, 1); }

	camera.x += (camera.targetx - camera.x - win.sw2) * 10.0f * dt;
	camera.y += (camera.targety - camera.y - win.sh2) * 10.0f * dt;

}

void render() { 
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  	SDL_RenderClear(renderer);

	render_world(true);
	render_player(true);
	render_particles(true);
	render_ui(true);

  	SDL_RenderPresent(renderer);
}

// ----------------------------------------------------------------------------------------------------

void AppQuit() {
	// save_world("gamesaves/world.save");
	destroy_particles();
    SDL_DestroyRenderer(renderer);
	renderer = NULL;
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void AppIterate() {
	SDL_GetWindowSize(window, &win.sw, &win.sh);
	win.sw2 = win.sw/2; win.sh2 = win.sh/2;
  	dt = get_delta_time();
	update();
  	render();
	control_fps();
}

void AppInit() {
	
    running = true;
	srand(time(NULL));

    if (!SDL_Init(SDL_INIT_VIDEO)) {
    	printf("Error initializing SDL: %s", SDL_GetError());
    	running = false;
  	}

  	window = SDL_CreateWindow("Stone Haven", win.sw, win.sh, SDL_WINDOW_RESIZABLE);
  	if (!window) {
    	printf("Error creating window: %s", SDL_GetError());
    	running = false;
  	}

  	renderer = SDL_CreateRenderer(window, NULL);
	SDL_SetRenderScale(renderer, 1, 1);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  	if (!renderer) {
    	printf("Error creating renderer: %s", SDL_GetError());
    	running = false;
  	}

	if (!load_textures(renderer)) {
	    printf("Error loading textures: %s", SDL_GetError());
	    running = false;
	}

    // SDL_Surface *icon = IMG_Load("assets/icon.ico");  // Path to your icon.ico file
    // if (!icon) {
    //     // printf("Error loading icon: %s\n", IMG_GetError());
    //     running = false;
    // }
	// SDL_SetWindowIcon(window, icon);

	make_blocks();
	if (!load_world("gamesaves/world.save")) {
			miner.x = WORLD_WIDTH*32;
			miner.y = WORLD_HEIGHT*32;
			miner.vx = 0;
			miner.vy = 0;
		if (!generate_world()) {
			printf("Error generating world: %s", SDL_GetError());
			running = false;
		}
	}

	camera.x = miner.x;
	camera.y = miner.y;

}

int main(int argc, char **argv) {
    AppInit();

    while (running) { AppIterate(); }

	AppQuit();

    return 0;

}
