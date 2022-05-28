#include "particle_sim.h"

#include <GL/gl.h>
#include <GL/glext.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "logger.h"

struct particle_t
{
	float pos[2];
	float vel[2];
};

GLuint
compile_shader_from_path(char const* path, GLenum shader_type) {
	FILE* file = fopen(path, "r");
	if(file == NULL) {
		ERR("File could not be opened: %s\n", path);
		return 0;
	}

	fseek(file, 0, SEEK_END);
	size_t size = ftell(file);

	size_t source_size = sizeof(GLchar) * (size+1);
	GLchar* source = malloc(source_size);
	fseek(file, 0, SEEK_SET);
	fread((void*)source, sizeof(GLchar), size, file);
	source[source_size-1] = '\0';

	assert(shader_type == GL_VERTEX_SHADER || shader_type == GL_FRAGMENT_SHADER);
	GLuint shader = glCreateShader(shader_type);
	assert(shader != 0);

	glShaderSource(shader, 1, (GLchar const**)&source, NULL);
	glCompileShader(shader);

#ifndef NDEBUG
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(success == GL_FALSE) {
		GLchar info_log[512];
		glGetShaderInfoLog(shader, 512, NULL, (GLchar*)&info_log);

		ERR("Error while compiling %s: \n", path);
		fprintf(stderr, "%s\n", info_log);

		glDeleteShader(shader);
		return 0;
	}
#endif

	return shader;
}

void
particle_sim_init(struct particle_sim_t* particle_sim, size_t count) {
	particle_sim->count = count;
	particle_sim->mass_center[0] = 0.f;
	particle_sim->mass_center[1] = 0.f;

	// Create shaders
	GLuint vert_shader = compile_shader_from_path("./res/shaders/particle_simulator.vert", GL_VERTEX_SHADER);
	GLuint frag_shader = compile_shader_from_path("./res/shaders/particle_simulator.frag", GL_FRAGMENT_SHADER);
	if(vert_shader == 0 || frag_shader == 0) {
		exit(EXIT_FAILURE);
	}

	// Shader program
	particle_sim->program = glCreateProgram();
	assert(particle_sim->program != 0);

	glAttachShader(particle_sim->program, vert_shader);
	glAttachShader(particle_sim->program, frag_shader);
	glLinkProgram(particle_sim->program);

	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);
#ifndef NDEBUG
	GLint success;
	glGetProgramiv(particle_sim->program, GL_LINK_STATUS, &success);
	if(success == GL_FALSE) {
		GLchar info_log[512];
		glGetProgramInfoLog(particle_sim->program, 512, NULL, (GLchar*)&info_log);
		ERR("Error linking shaders\n");
		fprintf(stderr, "%s", info_log);
		exit(EXIT_FAILURE);
	}
#endif
	glUseProgram(particle_sim->program);

	particle_sim->count_loc = glGetUniformLocation(particle_sim->program, "count");
	particle_sim->dtime_loc = glGetUniformLocation(particle_sim->program, "dtime");
	particle_sim->scale_vec_loc = glGetUniformLocation(particle_sim->program, "scale_vec");
	particle_sim->mass_center_loc = glGetUniformLocation(particle_sim->program, "mass_center");
	particle_sim->center_mass_loc = glGetUniformLocation(particle_sim->program, "center_mass");

	glUniform1ui(particle_sim->count_loc, particle_sim->count);
	glUniform1f(particle_sim->dtime_loc, 0.f);
	glUniform2f(particle_sim->mass_center_loc, 0.f, 0.f);
	glUniform2f(particle_sim->scale_vec_loc, 400.f, 300.f);
	glUniform1f(particle_sim->center_mass_loc, 30000.f);

	// Make vao buffer
	glGenVertexArrays(1, &particle_sim->vao);

	// Make ssbo buffer
	glGenBuffers(1, &particle_sim->ssbo);
	assert(particle_sim->ssbo != 0);

	// Generate random particles
	size_t particles_size = sizeof(struct particle_t) * count;
	struct particle_t* particles = malloc(particles_size);

	srand(time(NULL));
	for(uint32_t i=0;i<count;i++) {
		particles[i].pos[0] = ((float)(rand() % 800))-400.f;
		particles[i].pos[1] = (-(float)(rand() % 600))+300.f;

		particles[i].vel[0] = 0.f;
		particles[i].vel[1] = 0.f;
		//particles[i].vel[0] = (float)(rand() % 1000)/1000.f - 0.5f;
		//particles[i].vel[1] = (float)(rand() % 1000)/1000.f - 0.5f;
	}

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particle_sim->ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, particles_size, particles, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void
particle_sim_destroy(struct particle_sim_t* particle_sim)
{
	glDeleteBuffers(1, &particle_sim->ssbo);
}

void
particle_sim_draw(struct particle_sim_t const* particle_sim)
{
	glUseProgram(particle_sim->program);
	glBindVertexArray(particle_sim->vao);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particle_sim->ssbo);

	glDrawArrays(GL_POINTS, 0, particle_sim->count);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void
particle_sim_update(struct particle_sim_t* particle_sim, float dtime)
{
	glUniform1f(particle_sim->dtime_loc, dtime);
}

void
particle_sim_set_mass_center(struct particle_sim_t* particle_sim, float x, float y)
{
	glUniform2f(particle_sim->mass_center_loc, x, y);
}

void
particle_sim_set_scale_vec(struct particle_sim_t* particle_sim, float x, float y)
{
	glUniform2f(particle_sim->scale_vec_loc, x, y);
}

void
particle_sim_set_center_mass(struct particle_sim_t* particle_sim, float mass)
{
	glUniform1f(particle_sim->center_mass_loc, mass);
}
