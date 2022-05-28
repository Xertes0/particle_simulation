#version 460 core

uniform uint count;
uniform float dtime;
uniform vec2 mass_center;
uniform vec2 scale_vec;
uniform float center_mass;

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

	vec2 diff = mass_center - particle.pos;
	float dist = clamp(sqrt((diff.x*diff.x) + (diff.y*diff.y)), 10, 10000);
	float force = center_mass/dist;
	vec2 unit_vec = diff/dist;

	particle.vel += unit_vec * force * dtime;
	particle.pos += particle.vel * dtime;
	ssbo.particles[gl_VertexID] = particle;


	io_data.color = vec4(
			max(0.05, abs(particle.vel[0]/200)),
			max(0.05, abs(particle.vel[1]/150)),
			//max(0.05, abs(particle.vel[0]*particle.vel[1])/1000),
			max(0.1, abs(1000/dist) - 0.1),
			1.0);
	gl_Position = vec4(particle.pos/scale_vec, 0.0, 1.0);
}
