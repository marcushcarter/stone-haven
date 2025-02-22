
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <windows.h>

#include "types.c"
#include "textures.c"
#include "common.c"
#include "world.c"
#include "inventory.c"
#include "particle.c"
#include "player.c"
#include "savefile.c"
#include "menu.c"

// ----------------------------------------------------------------------------------------------------

void editor_controls(bool active) {
	if (active) {

		if (key.n1) set.gamemode = GM_SURVIVAL;
		if (key.n2) set.gamemode = GM_CREATIVE;
		if (key.n3) set.gamemode = GM_FREECAM;

		// if (key.p_) pause = !pause;
		if (key.p_) appstate = APP_MENU;

		if (key.space) {
			for (int i = 0; i < 10; i++) {
				create_particle(P_GRAVITY, miner.x, miner.y, randfloat(-1000, 1000), randfloat(-1000, 1000), 1, COLOR_WHITE);
			}
		}
	}
}

void alternate_controls(bool active) {

	if (key.tab) { SDL_SetWindowFullscreen(window, 1); }
	if (key.escape) { SDL_SetWindowFullscreen(window, 0); }

	static Uint32 last_save_time;
	if (SDL_GetTicks() / 1000 - last_save_time >= 60*5 && set.auto_save) {
		save_world("gamesaves/world.save");
		last_save_time = SDL_GetTicks() / 1000;
	}

	if (set.gamemode == GM_FREECAM) {
		set.brightness = 10.0;
	} else {
		set.brightness = 5.0;
	}
}

void update() {

	update_keystates(true);
	
	if (appstate == APP_PLAY) {
		editor_controls(true);
		if (!pause) {
			if (dt > 0.3) return;
			statistics.seconds_played+=dt;
			alternate_controls(true);
			update_blocks(true);
			update_player(true);
			update_particles(true);
		}
	}

	camera.x += (camera.targetx - camera.x - win.sw2) * 10.0f * dt;
	camera.y += (camera.targety - camera.y - win.sh2) * 10.0f * dt;

}

void render() { 
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  	SDL_RenderClear(renderer);

	if (appstate == APP_PLAY) {
		render_world(true);
		render_player(true);
		render_particles(true);
		render_ui(true);
	} else {
		render_update_menu(true);
	}

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
	if (win.sw < 414) SDL_SetWindowSize(window, 414, win.sh);
	SDL_GetWindowSize(window, &win.sw, &win.sh);
	if (win.sh < 253) SDL_SetWindowSize(window, win.sw, 253);
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
		return;
  	}

	  if (TTF_Init() == -1) {
        // printf("SDL_ttf could not initialize! TTF_Error: %s\n", TTF_GetError());
        running = false;
		return;
    }

  	window = SDL_CreateWindow("Stone Haven", win.sw, win.sh, SDL_WINDOW_RESIZABLE);
  	if (!window) {
    	printf("Error creating window: %s", SDL_GetError());
    	running = false;
		return;
  	}

  	renderer = SDL_CreateRenderer(window, NULL);
	SDL_SetRenderScale(renderer, 1, 1);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  	if (!renderer) {
    	printf("Error creating renderer: %s", SDL_GetError());
    	running = false;
		return;
  	}

	if (!load_textures(renderer)) {
	    printf("Error loading textures: %s", SDL_GetError());
	    running = false;
		return;
	}

	if (!make_blocks()) {
		printf("Error making blocks");
		running = false;
		return;
	}

	make_menu_tapestry();

	camera.x += (miner.x - win.sw2);
	camera.y += (miner.y - win.sh2);

}

int main(int argc, char **argv) {
    AppInit();

    while (running) { AppIterate(); }

	AppQuit();

    return 0;

}
