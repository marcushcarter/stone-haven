
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
        // Find an empty spot in the particles array
        bool particle_created = false;
        for (int i = 0; i < MAX_PARTICLES; i++) {
            if (particles[i] == NULL) {
                particles[i] = (Particle*)malloc(sizeof(Particle));
                if (particles[i] == NULL) {
                    printf("Failed to allocate memory for particle\n");
                    return;
                }

                // Initialize the particle
                particles[i]->type = type;
                particles[i]->x = x;
                particles[i]->y = y;
                particles[i]->vx = vx;
                particles[i]->vy = vy;
                particles[i]->life = life;
                particles[i]->color = color;

                particle_created = true;
                // printf("Creating particle at %f, %f\n", x, y);
                break; // Break out of the loop once a particle is created
            }
        }
        
        // If no empty spot was found, stop trying to create more particles
        if (!particle_created) {
            printf("No available space to create more particles\n");
            return;
        }
    
}

void update_particles(bool active) {

    if (active && set.particles) {

        for (int i = 0; i < MAX_PARTICLES; i++) {
            if (particles[i] == NULL) continue;
            if (distance2d(camera.x+(win.sw2), camera.y+(win.sh2), particles[i]->x, particles[i]->y) > set.update_distance*64) continue;

            particles[i]->life -= 1*dt;
            int iterations = 15;
            bool particle_deleted = false;
            for (int j = 0; j < iterations; j++) {

                if (particles[i]->life <= 0 || world[(int)round(particles[i]->x/64)][(int)round(particles[i]->y/64)]->solid) {
                    free(particles[i]);
                    particles[i] = NULL;
                    break;
                }
                
                if (particles[i]->type == P_GRAVITY) {
                    // particles[i]->vx +=
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
            if (particle_deleted) continue;
        }
    }
}

void render_particles(bool active) {
    if (active && set.particles) {
        for (int i = 0; i < MAX_PARTICLES; i++) {
            if (particles[i] == NULL) continue;
            if ((particles[i]->y-64) - camera.y > win.sh || (particles[i]->y+64) - camera.y < 0 || (particles[i]->x+64) - camera.x < 0 || (particles[i]->x-64) - camera.x > win.sw ) continue;
            draw_rect(renderer, floatarr(4, particles[i]->x - camera.x, particles[i]->y - camera.y, 10.0f, 10.0f), particles[i]->color, 255, true);
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