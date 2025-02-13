
typedef enum {
    P_GRAVITY,
    P_EASE,
    P_FLOAT,
    P_FALL,
} ParticleType;

typedef struct {
    ParticleType type;
    float life;
    float x, y, vx, vy;
    int color;
} Particle;

Particle* particles[MAX_PARTICLES] = {NULL};

void create_particle(ParticleType type, float x, float y, float vx, float vy, float life, int color) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i] == NULL) {

            particles[i] = (Particle*)malloc(sizeof(Particle));
            if (particles[i] == NULL) {
                printf("Failed to allocate memory for particle\n");
                return;
            }

            particles[i]->type = type;
            particles[i]->x = x;
            particles[i]->y = y;
            particles[i]->vx = vx;
            particles[i]->vy = vy;
            particles[i]->life = life;
            particles[i]->color = color;

            return;
        }
    }
    printf("No available space to create more particles\n");
    return;
}

void resolve_particle_collision(int i, CollisionType type, IntersectionResult result, float x, float y) {

	/*	Explanation time:
	       -basically just gets the coordinates of the collision point
			and moves the particle to that point if any of their collision 
			lines are intersecting with a specific blocks collision lines
	*/

	if (result.isIntersecting) {

		if (type == COLLISION_BOTTOM) {
			particles[i]->y = result.cy - 5;
            particles[i]->vx *= 0.995;
			particles[i]->vy *= -0.25;

		}

		if (type == COLLISION_TOP) {
			particles[i]->y = result.cy + 5;
            particles[i]->vx *= 0.995;
			particles[i]->vy *= -0.25;
		}

		if (type == COLLISION_RIGHT) {
			particles[i]->x = result.cx - 5;
            particles[i]->vx *= -0.25;
		}

		if (type == COLLISION_LEFT) {
			particles[i]->x = result.cx + 5;
            particles[i]->vx *= -0.25;
		}
	}
}

void check_particle_block(int i, int x, int y) {

	/*  A partcile's hitbox consists of:
		    - A line pointing up to detect if the particle is touching any cieling blocks
		    - A line pointing down to check if the particle is touching any floor blocks
		    - A line pointing left to check if the particle is touching any left walls
		    - A line pointing right to check if the particle is touching any right walls
	*/

	if (world[x][y]->solid == false || world[x][y] == NULL) return;
	if (x < 0 || y < 0 || x >= WORLD_WIDTH || y >= WORLD_HEIGHT) return;

	IntersectionResult result;
    
    Line blkt = {x*64 - 32, y*64 - 32, x*64 +32, y*64 -32};
	Line blkb = {x*64 - 32, y*64 + 32, x*64 +32, y*64 +32};
	Line blkr = {x*64 + 32, y*64 + 32, x*64 + 32, y*64 - 32};
	Line blkl = {x*64 - 32, y*64 + 32, x*64 -32, y*64 -32};

    Line pb = {particles[i]->x, particles[i]->y, particles[i]->x, particles[i]->y+5};
	result = islinesintersecting(pb, blkt);
    if (result.isIntersecting) resolve_particle_collision(i, COLLISION_BOTTOM, result, x, y);

    Line pt = {particles[i]->x, particles[i]->y-5, particles[i]->x, particles[i]->y};
	result = islinesintersecting(pt, blkb);
    if (result.isIntersecting) resolve_particle_collision(i, COLLISION_TOP, result, x, y);

    Line pl = {particles[i]->x-5, particles[i]->y, particles[i]->x, particles[i]->y};
	result = islinesintersecting(pl, blkr);
    if (result.isIntersecting) resolve_particle_collision(i, COLLISION_LEFT, result, x, y);

    Line pr = {particles[i]->x, particles[i]->y, particles[i]->x+5, particles[i]->y};
	result = islinesintersecting(pr, blkl);
    if (result.isIntersecting) resolve_particle_collision(i, COLLISION_RIGHT, result, x, y);

}	

void check_particle_collision(int i) {

	/* 	Only checks for collision with blocks that are: 
		    - in the same tile as the particle
		    - above and below the particle
		    - to the left and right of the particle
		    - in all 4 diagonals to the particle
	*/

	// block inside the particle
	check_particle_block(i, (int)(particles[i]->x/64), (int)(particles[i]->y/64));

	// blocks above, below, and next to the particle
	check_particle_block(i, (int)(particles[i]->x/64), (int)(particles[i]->y/64)-1); // blocks around the particle
	check_particle_block(i, (int)(particles[i]->x/64), (int)(particles[i]->y/64)+1);
	check_particle_block(i, (int)(particles[i]->x/64)-1, (int)(particles[i]->y/64));
	check_particle_block(i, (int)(particles[i]->x/64)+1, (int)(particles[i]->y/64));

 	// blocks diagonal to the particle
	check_particle_block(i, (int)(particles[i]->x/64)-1, (int)(particles[i]->y/64)-1);
	check_particle_block(i, (int)(particles[i]->x/64)+1, (int)(particles[i]->y/64)-1);
	check_particle_block(i, (int)(particles[i]->x/64)+1, (int)(particles[i]->y/64)+1);
	check_particle_block(i, (int)(particles[i]->x/64)-1, (int)(particles[i]->y/64)+1);
}

void update_particles(bool active) {

    if (active && set.particles && set.gamemode != GM_FREECAM) {

        for (int i = 0; i < MAX_PARTICLES; i++) {
            if (particles[i] == NULL) continue;
            if (distance2d(camera.x+(win.sw2), camera.y+(win.sh2), particles[i]->x, particles[i]->y) > set.update_distance*64) continue;

            particles[i]->life -= 1*dt;
            int iterations = 15;
            for (int j = 0; j < iterations; j++) {

                if (particles[i]->life <= 0) {
                    free(particles[i]);
                    particles[i] = NULL;
                    break;
                }
                
                if (particles[i]->type == P_GRAVITY) {
                    particles[i]->vy += set.gravity * dt/iterations;
                    particles[i]->x += particles[i]->vx * dt/iterations;
                    particles[i]->y += particles[i]->vy * dt/iterations;
                }

                if (particles[i]->type == P_EASE) {
                    particles[i]->vx *= 0.99;
                    particles[i]->vy *= 0.99;
                    particles[i]->x += particles[i]->vx * dt/iterations;
                    particles[i]->y += particles[i]->vy * dt/iterations;
                }

                check_particle_collision(i);

            }
        }
    }
}

void render_particles(bool active) {
    if (active && set.particles) {
        for (int i = 0; i < MAX_PARTICLES; i++) {
            if (particles[i] == NULL) continue;
            if ((particles[i]->y-64) - camera.y > win.sh || (particles[i]->y+64) - camera.y < 0 || (particles[i]->x+64) - camera.x < 0 || (particles[i]->x-64) - camera.x > win.sw ) continue;

            float transparency = 255.0f;
            if (particles[i]->life <= 0.25) {
                transparency = max(0.0f, 255 * (particles[i]->life / 0.25));
            }

            float brightness;
            float dx = particles[i]->x - miner.x; float dy = particles[i]->y - miner.y;
            brightness = 220.0f - ((sqrt(dx * dx + dy * dy) / 64) / set.brightness) * 220.0f;
            if ((sqrt(dx * dx + dy * dy) / 64) > set.brightness) brightness = 0;
            if (brightness < 0) { brightness = 0; } else if (brightness > 220.0f) { brightness = 220.0f; }
            brightness = 250.0f-abs(brightness);

            transparency -= brightness;
            if (transparency > 220) { transparency = 220; } else if (transparency < 0) { transparency = 0; }
            
            draw_rect(renderer, floatarr(4, particles[i]->x - camera.x - 5, particles[i]->y - camera.y - 5, 10.0f, 10.0f), particles[i]->color, transparency, true);
        
        }
    }
}

void destroy_particles() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i] != NULL) {
            free(particles[i]);
            particles[i] = NULL;
        }
    }
}