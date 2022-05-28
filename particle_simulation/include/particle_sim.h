#ifndef PS_PARTICLE_SIM_H
#define PS_PARTICLE_SIM_H

#include <stdlib.h>

#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1
#include <GL/gl.h>

struct particle_sim_t
{
	size_t count;
	GLuint vao;
	GLuint ssbo;
	GLuint program;

	GLint count_loc;
	GLint dtime_loc;
	GLint mass_center_loc;
	GLint scale_vec_loc;
	float mass_center[2];
};

void
particle_sim_init(struct particle_sim_t* particle_sim, size_t count);

void
particle_sim_destroy(struct particle_sim_t* particle_sim);

void
particle_sim_draw(struct particle_sim_t const* particle_sim);

void
particle_sim_update(struct particle_sim_t* particle_sim, float dtime);

void
particle_sim_set_mass_center(struct particle_sim_t* particle_sim, float x, float y);

void
particle_sim_set_scale_vec(struct particle_sim_t* particle_sim, float x, float y);

#endif // PS_PARTICLE_SIM_H
