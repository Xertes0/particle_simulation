#include "particle_sim.h"

#include <GL/gl.h>

#include <stdlib.h>
#include <string.h>
#include <time.h>

struct particle_t
{
	float pos[2];
	float vel[2];
};

void
particle_sim_init(struct particle_sim_t* particle_sim, size_t count) {
	particle_sim->count = count;

	// Make ssbo buffer
	glGenBuffers(1, &particle_sim->ssbo);

	// Generate random particles
	size_t particles_size = sizeof(struct particle_t) * count;
	struct particle_t* particles = malloc(particles_size);
	memset((void*)particles, 0, sizeof(struct particle_t) * count);

	srand(time(NULL));
	for(uint32_t i=0;i<count;i++) {
		particles[i].pos[0] = rand() % 800;
		particles[i].pos[1] = -(rand() % 600);
	}

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particle_sim->ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, particles_size, particles, GL_DYNAMIC_COPY);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void
particle_sim_destroy(struct particle_sim_t* particle_sim)
{
	glDeleteBuffers(1, &particle_sim->ssbo);
}
