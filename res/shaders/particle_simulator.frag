#version 460 core

layout(location=0) out vec4 frag_color;

in io_data_t
{
	vec4 color;
} io_data;

void main()
{
	frag_color = io_data.color;
}
