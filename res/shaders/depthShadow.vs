#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;
layout (location = 2) in vec3 normal;

out vec3 vs_position;
out vec2 vs_texcoord;
out vec3 vs_normal;


void main()
{
    vs_position = position;
    vs_texcoord = texcoord;
    vs_normal = normal;
}
