#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;

out vec3 vs_position;
out vec2 vs_texcoord;
out vec3 vs_normal;
out vec3 vs_tangent;

out vec4 vs_fragPosLightSpace;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;


void main()
{
    vs_position = position; //vec4(ModelMatrix * vec4(position, 1.f)).xyz;
    vs_texcoord = texcoord;
    vs_normal = normal; //mat3(ModelMatrix) * normal;
    vs_tangent = tangent;

    //gl_Position = vec4(position.xyz, 1.f);
}
