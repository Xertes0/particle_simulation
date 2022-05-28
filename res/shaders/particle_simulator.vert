#version 460 core

uniform uint count;
uniform float dtime;
uniform vec2 mass_center;
uniform vec2 scale_vec;

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

#define PARTICLE_MASS 1
#define CENTER_MASS 20000

void main()
{
	particle_t particle = ssbo.particles[gl_VertexID];

	vec2 diff = mass_center - particle.pos;
	float dist = clamp(sqrt((diff.x*diff.x) + (diff.y*diff.y)), 10, 10000);
	float force = (PARTICLE_MASS * CENTER_MASS)/dist;
	vec2 unit_vec = diff/dist;

	particle.vel += unit_vec * force * dtime;

	//for(uint i=0;i<count;i++) {
	//	if(i==gl_VertexID) {
	//		continue;
	//	}
	//	diff = ssbo.particles[i].pos - particle.pos;
	//	dist = clamp(sqrt((diff.x*diff.x) + (diff.y*diff.y)), 1, 10000);
	//	force = (PARTICLE_MASS * PARTICLE_MASS)/dist;
	//	unit_vec = diff/dist;

	//	particle.vel += unit_vec * force * dtime;
	//}

	particle.pos += particle.vel * dtime;
	ssbo.particles[gl_VertexID] = particle;


	io_data.color = vec4(abs(particle.vel[0]/400), abs(particle.vel[1]), abs(particle.vel[0]*particle.vel[1])/1000, 1.0);
	gl_Position = vec4(particle.pos/scale_vec, 0.0, 1.0);
}
