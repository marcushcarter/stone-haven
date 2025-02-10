
typedef struct {
	const Uint8 *keystate;
    unsigned char a : 1, b : 1, c : 1, d : 1, e : 1, f : 1, g : 1, h : 1, i : 1;
    unsigned char j : 1, k : 1, l : 1, m : 1, n : 1, o : 1, p : 1, q : 1, r : 1;
    unsigned char s : 1, t : 1, u : 1, v : 1, w : 1, x : 1, y : 1, z : 1;
    unsigned char n1 : 1, n2 : 1, n3 : 1, n4 : 1, n5 : 1, n6 : 1, n7 : 1, n8 : 1, n9 : 1, n0 : 1;
    unsigned char enter : 1, tab : 1, shift : 1, space : 1, escape : 1, ctrl : 1, alt : 1, backspace : 1; 
    unsigned char upa : 1, downa : 1, lefta : 1, righta : 1;
    unsigned char f1 : 1, f2 : 1, f3 : 1, f4 : 1, f5 : 1, f6 : 1, f7 : 1, f8 : 1, f9 : 1, f10 : 1, f11 : 1, f12 : 1;
} keyio; keyio key;

typedef struct {
	float x, y;
	float xrelative, yrelative;
	int worldx, worldy;
	Uint32 state, prev_state;
    Uint8 l, r, m;      // current mouse button states
    Uint8 prev_l, prev_r, prev_m; // previous mouse button states
    Uint8 lp, rp, mp;   // left, right, middle press (1 if pressed this frame)
} mouseio; mouseio mouse;

SDL_Window* window;
SDL_Renderer* renderer;
bool running;
SDL_Event event;

typedef struct {
  int sw, sh;
  int sw2, sh2;
} WindowDetails;

WindowDetails win = {1024, 768, 512, 384};

typedef enum {
	CD_KEYBOARD,
	CD_CONTROLLER,
} ControlDevice;

typedef struct {
  	int max_fps; //120.0f

  	bool fullscreen; //false
  	bool particles; //true
  	float brightness; //5.0f
  	ControlDevice controller; // keyboard or controller

  	bool creative;
  	bool hide_hud; //false
  	bool auto_save; // true

  	int master_vol; //100
  	int music_vol; //100
  	int sfx_vol; //100
  	int world_vol; //100

	int gravity; //1000.0f
} GameSettings;

GameSettings set = {
	120,
	false, true, 5, CD_KEYBOARD,
	true, false, true,
	100, 100, 100, 100,
	1000,
};

typedef struct {
	float x, y;
	float targetx, targety;
	bool freecam;
} Camera; Camera camera;

typedef struct {
    char name[100];

    float x, y;
    float vx, vy;
    float speed;
    float health;

    float width, height;

    float falling;
    float jumptimer;
    float breaktimer;
} Player;

Player miner = {    
                
                "Player",
                0.0f, 0.0f, 0.0f, 0.0f, 400.0f, 100.0f,
				// 0.0f, 0.0f, 0.0f, 0.0f, 500.0f, 100.0f,
                50.0f, 50.0f,
                0.0f, 0.0f, 0.0f
                
};

void update_keystates(bool active) {
	if (active) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
			}
		}

		SDL_GetMouseState(&mouse.x, &mouse.y);
		SDL_GetRelativeMouseState(&mouse.xrelative, &mouse.yrelative);
		mouse.worldx = (int)(round(mouse.x+camera.x+32)/64);
		mouse.worldy = (int)(round(mouse.y+camera.y+32)/64);

		mouse.prev_state = mouse.state;
		mouse.state = SDL_GetMouseState(NULL, NULL);

		mouse.l = mouse.state & SDL_BUTTON_MASK(SDL_BUTTON_LEFT);
		mouse.r = mouse.state & SDL_BUTTON_MASK(SDL_BUTTON_RIGHT);
		mouse.m = mouse.state & SDL_BUTTON_MASK(SDL_BUTTON_MIDDLE);

		mouse.prev_l = mouse.prev_state & SDL_BUTTON_MASK(SDL_BUTTON_LEFT);
		mouse.prev_r = mouse.prev_state & SDL_BUTTON_MASK(SDL_BUTTON_RIGHT);
		mouse.prev_m = mouse.prev_state & SDL_BUTTON_MASK(SDL_BUTTON_MIDDLE);

		if (!mouse.prev_l && mouse.l) { mouse.lp = 1; } else { mouse.lp = 0; }
		if (!mouse.prev_r && mouse.r) { mouse.rp = 1; } else { mouse.rp = 0; }
		if (!mouse.prev_m && mouse.m) { mouse.mp = 1; } else { mouse.mp = 0; }

		key.keystate = (const Uint8*)SDL_GetKeyboardState(NULL);  // Retrieve current keyboard state

		key.a = key.keystate[SDL_SCANCODE_A] ? 1 : 0;
		key.b = key.keystate[SDL_SCANCODE_B] ? 1 : 0;
		key.c = key.keystate[SDL_SCANCODE_C] ? 1 : 0;
		key.d = key.keystate[SDL_SCANCODE_D] ? 1 : 0;
		key.e = key.keystate[SDL_SCANCODE_E] ? 1 : 0;
		key.f = key.keystate[SDL_SCANCODE_F] ? 1 : 0;
		key.g = key.keystate[SDL_SCANCODE_G] ? 1 : 0;
		key.h = key.keystate[SDL_SCANCODE_H] ? 1 : 0;
		key.i = key.keystate[SDL_SCANCODE_I] ? 1 : 0;
		key.j = key.keystate[SDL_SCANCODE_J] ? 1 : 0;
		key.k = key.keystate[SDL_SCANCODE_K] ? 1 : 0;
		key.l = key.keystate[SDL_SCANCODE_L] ? 1 : 0;
		key.m = key.keystate[SDL_SCANCODE_M] ? 1 : 0;
		key.n = key.keystate[SDL_SCANCODE_N] ? 1 : 0;
		key.o = key.keystate[SDL_SCANCODE_O] ? 1 : 0;
		key.p = key.keystate[SDL_SCANCODE_P] ? 1 : 0;
		key.q = key.keystate[SDL_SCANCODE_Q] ? 1 : 0;
		key.r = key.keystate[SDL_SCANCODE_R] ? 1 : 0;
		key.s = key.keystate[SDL_SCANCODE_S] ? 1 : 0;
		key.t = key.keystate[SDL_SCANCODE_T] ? 1 : 0;
		key.u = key.keystate[SDL_SCANCODE_U] ? 1 : 0;
		key.v = key.keystate[SDL_SCANCODE_V] ? 1 : 0;
		key.w = key.keystate[SDL_SCANCODE_W] ? 1 : 0;
		key.x = key.keystate[SDL_SCANCODE_X] ? 1 : 0;
		key.y = key.keystate[SDL_SCANCODE_Y] ? 1 : 0;
		key.z = key.keystate[SDL_SCANCODE_Z] ? 1 : 0;
		key.n1 = key.keystate[SDL_SCANCODE_1] ? 1 : 0;
		key.n2 = key.keystate[SDL_SCANCODE_2] ? 1 : 0;
		key.n3 = key.keystate[SDL_SCANCODE_3] ? 1 : 0;
		key.n4 = key.keystate[SDL_SCANCODE_4] ? 1 : 0;
		key.n5 = key.keystate[SDL_SCANCODE_5] ? 1 : 0;
		key.n6 = key.keystate[SDL_SCANCODE_6] ? 1 : 0;
		key.n7 = key.keystate[SDL_SCANCODE_7] ? 1 : 0;
		key.n8 = key.keystate[SDL_SCANCODE_8] ? 1 : 0;
		key.n9 = key.keystate[SDL_SCANCODE_9] ? 1 : 0;
		key.n0 = key.keystate[SDL_SCANCODE_0] ? 1 : 0;

		// Special keys
		key.enter = key.keystate[SDL_SCANCODE_RETURN] ? 1 : 0;
		key.tab = key.keystate[SDL_SCANCODE_TAB] ? 1 : 0;
		key.shift = key.keystate[SDL_SCANCODE_LSHIFT] || key.keystate[SDL_SCANCODE_RSHIFT] ? 1 : 0;
		key.space = key.keystate[SDL_SCANCODE_SPACE] ? 1 : 0;
		key.escape = key.keystate[SDL_SCANCODE_ESCAPE] ? 1 : 0;
		key.ctrl = key.keystate[SDL_SCANCODE_LCTRL] || key.keystate[SDL_SCANCODE_RCTRL] ? 1 : 0;
		key.alt = key.keystate[SDL_SCANCODE_LALT] || key.keystate[SDL_SCANCODE_RALT] ? 1 : 0;
		key.backspace = key.keystate[SDL_SCANCODE_BACKSPACE] ? 1 : 0;

		// Arrow keys
		key.upa = key.keystate[SDL_SCANCODE_UP] ? 1 : 0;
		key.downa = key.keystate[SDL_SCANCODE_DOWN] ? 1 : 0;
		key.lefta = key.keystate[SDL_SCANCODE_LEFT] ? 1 : 0;
		key.righta = key.keystate[SDL_SCANCODE_RIGHT] ? 1 : 0;

		// Function keys (F1-F12)
		key.f1 = key.keystate[SDL_SCANCODE_F1] ? 1 : 0;
		key.f2 = key.keystate[SDL_SCANCODE_F2] ? 1 : 0;
		key.f3 = key.keystate[SDL_SCANCODE_F3] ? 1 : 0;
		key.f4 = key.keystate[SDL_SCANCODE_F4] ? 1 : 0;
		key.f5 = key.keystate[SDL_SCANCODE_F5] ? 1 : 0;
		key.f6 = key.keystate[SDL_SCANCODE_F6] ? 1 : 0;
		key.f7 = key.keystate[SDL_SCANCODE_F7] ? 1 : 0;
		key.f8 = key.keystate[SDL_SCANCODE_F8] ? 1 : 0;
		key.f9 = key.keystate[SDL_SCANCODE_F9] ? 1 : 0;
		key.f10 = key.keystate[SDL_SCANCODE_F10] ? 1 : 0;
		key.f11 = key.keystate[SDL_SCANCODE_F11] ? 1 : 0;
		key.f12 = key.keystate[SDL_SCANCODE_F12] ? 1 : 0;
	}
}

// #define WORLD_WIDTH 2000
// #define WORLD_HEIGHT 500

#define WORLD_WIDTH 1000
#define WORLD_HEIGHT 1000

#define MAX_PARTICLES 100000