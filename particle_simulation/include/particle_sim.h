#ifndef PS_PARTICLE_SIM_H
#define PS_PARTICLE_SIM_H

#include <stdlib.h>

#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1
#include <GL/gl.h>

struct particle_sim_t
{
	size_t count;
	GLuint ssbo;
};

void
particle_sim_init(struct particle_sim_t* particle_sim, size_t count);

void
particle_sim_destroy(struct particle_sim_t* particle_sim);

#endif // PS_PARTICLE_SIM_H
