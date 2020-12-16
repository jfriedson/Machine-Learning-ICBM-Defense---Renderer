#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;
layout (location = 2) in vec3 normal;

out vec3 vs_position;
out vec2 vs_texcoord;
out vec3 vs_normal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;


void main()
{
    vs_position = vec4(ModelMatrix * vec4(position, 1.f)).xyz;
    vs_texcoord = texcoord;
    vs_normal = normalize(mat3(ModelMatrix) * normal);

    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1.f);
}