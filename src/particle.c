
typedef enum {
    P_IMPACT,
    P_HURT,
    P_LAVA,
    P_FIRE,
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
    if (active) {

        for (int i = 0; i < MAX_PARTICLES; i++) {
            if (particles[i] == NULL) continue;

            particles[i]->life -= 1*dt;

            if (particles[i]->life <= 0) {
                free(particles[i]);
                particles[i] = NULL;
                continue;
            }
                
            if (particles[i]->type == P_IMPACT ) {
                // particles[i]->vx +=
                particles[i]->vy += set.gravity * dt;
                particles[i]->x += particles[i]->vx * dt;
                particles[i]->y += particles[i]->vy * dt;
            }
        }











    }
}

void render_particles(bool active) {
    if (active) {
        for (int i = 0; i < MAX_PARTICLES; i++) {
            if (particles[i] == NULL) continue;
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