#version 460 core

struct particle_t
{
	vec2 pos;
	vec2 vel;
};

layout (std430, binding=0) restrict buffer particles_ssbo_t
{
	particle_t particles[];
} ssbo;

out io_data_t
{
	vec4 color;
} io_data;

void main()
{
	particle_t particle = ssbo.particles[gl_VertexID];

	io_data.color = vec4(particle.pos, 1.0, 1.0);
	gl_Position = vec4(particle.pos, 0.0, 1.0);
	//gl_Position = vec4(0.4, 0.4, 0.0, 1.0);
}
