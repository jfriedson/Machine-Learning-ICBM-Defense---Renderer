#version 460

layout(location = 0) out vec4 fs_color;

in vec3 vs_position;
in vec2 vs_texcoord;
in vec3 vs_normal;

uniform sampler2D cloudsTex;
uniform vec2 cloud_offset;

uniform vec3 camPos;


void main()
{
    // Normalize
    vec3 normal = normalize(vs_normal);

    //vec3 noise_seed = vec3(vs_texcoord, cloud_offset);
    //float cloud_noise = max(cloudNoise(noise_seed, 25.f, 5.f, 2, .7f), 0.f);

    // dont show at the poles to hide artifacts
    float cloud_bias = max(.8f - 2.f * abs(vs_texcoord.y + .5f), 0.f);

    // transparent at horizon
    vec3 viewDir = normalize(camPos - vs_position);
    cloud_bias *= max(dot(normal, viewDir), 0.02f);

    float clouds = cloud_bias * texture2D(cloudsTex, vs_texcoord + cloud_offset).r;


    fs_color = vec4(vec3(1.f), clouds);
}
