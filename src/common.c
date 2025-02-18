
IntersectionResult islinesintersecting(Line l1, Line l2) { // source: https://www.jeffreythompson.org/collision-detection/line-line.php
    IntersectionResult result;
    result.isIntersecting = false;

    // Calculate the denominator (determinant)
    float denom = ((l2.y2 - l2.y1) * (l1.x2 - l1.x1)) - ((l2.x2 - l2.x1) * (l1.y2 - l1.y1));

    // If the denominator is close to zero, lines are parallel or coincident
    if (fabs(denom) < 1e-6) return result;

    // Calculate uA and uB
    float uA = (((l2.x2 - l2.x1) * (l1.y1 - l2.y1)) - ((l2.y2 - l2.y1) * (l1.x1 - l2.x1))) / denom;
    float uB = (((l1.x2 - l1.x1) * (l1.y1 - l2.y1)) - ((l1.y2 - l1.y1) * (l1.x1 - l2.x1))) / denom;

    // Calculate intersection coordinates
    float cx = l1.x1 + (uA * (l1.x2 - l1.x1));
    float cy = l1.y1 + (uA * (l1.y2 - l1.y1));

    // Check if the intersection is within both line segments
    if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {
        result.isIntersecting = true;
        result.uA = uA;
        result.uB = uB;
        result.cx = cx;
        result.cy = cy;
    }

    return result;
}

float get_delta_time() {
	clock_t current_time = clock();
	float delta_time = (float)(current_time - previous_time) / CLOCKS_PER_SEC;
	previous_time = current_time;
	return delta_time;
}

void control_fps() {
	clock_t frame_start_time = clock();
	while ((float)(clock() - frame_start_time) / CLOCKS_PER_SEC < (1.0 / set.max_fps)) {}    
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

void create_particle(ParticleType type, float x, float y, float vx, float vy, float life, int color);