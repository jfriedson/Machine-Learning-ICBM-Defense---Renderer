#version 460

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;

out vec3 vs_position;
out vec2 vs_texcoord;
out vec3 vs_normal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;


void main()
{
	vs_position = vec4(ModelMatrix * vec4(pos, 1.f)).xyz;
	vs_texcoord = tex;
	vs_normal = mat3(ModelMatrix) * norm;

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(pos, 1.f);
}
