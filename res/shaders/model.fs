#version 460

in vec3 vs_position;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

uniform sampler2D diffuseTex;


void main()
{
	fs_color = texture(diffuseTex, vs_texcoord);
}
