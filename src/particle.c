
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

void update_particles(bool active) {

    if (active && set.particles) {

        for (int i = 0; i < MAX_PARTICLES; i++) {
            if (particles[i] == NULL) continue;
            if (distance2d(camera.x+(win.sw2), camera.y+(win.sh2), particles[i]->x, particles[i]->y) > set.update_distance*64) continue;

            particles[i]->life -= 1*dt;
            int iterations = 15;
            for (int j = 0; j < iterations; j++) {

                if (particles[i]->life <= 0 || world[(int)round(particles[i]->x/64)][(int)round(particles[i]->y/64)]->solid) {
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