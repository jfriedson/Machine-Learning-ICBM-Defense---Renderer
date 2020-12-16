#version 460

layout(location = 0) out vec4 fs_color;

in vec3 te_position;
in vec2 te_texcoord;
in vec3 te_normal;

in mat3 te_tangentSpace;
in vec4 te_fragPosLightSpace;

uniform sampler2D diffuseTex;
uniform sampler2D specTex;
uniform sampler2D normTex;
uniform sampler2D lightsTex;
uniform sampler2DShadow shadowTex;

uniform vec3 lightDir;
uniform vec3 camPos;


float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5f + 0.5f;

    // outside of shadowmap fully unlit
    if(projCoords.z > 1.0)
        return 1.f;

    //float closestDepth = texture(shadowTex, projCoords.xy).r;
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005); 
    
    float shadow = 0.f;
    vec2 texelSize = 1.f / textureSize(shadowTex, 0);
    for(int x = -1; x <= 1; x++)
    {
        for(int y = -1; y <= 1; y++)
        {
            float pcfDepth = texture(shadowTex, vec3(projCoords.xy + vec2(x, y) * texelSize, projCoords.z + 0.00001f ));
            shadow += projCoords.z - bias > pcfDepth ? 1.f : 0.f;
        }    
    }
    shadow /= 9.f;

    return shadow;
}

void main()
{
    vec3 normal = te_normal;
    vec3 normal_map = te_tangentSpace * normalize(texture(normTex, te_texcoord).rgb * 2.f - 1.f);
    if(dot(normal_map, vec3(0.f, .7074f, .7074f)) < .999f)
        normal = normal_map;

    // Basic ambient, specular, diffuse
    vec3 sunColor = vec3(.97f, .80f, .65f);

    vec3 viewDir = normalize(camPos - te_position);
    vec3 reflectDir =  reflect(-lightDir, normal);

    vec3 ambient = sunColor * .1f;

    float spec = pow(max(dot(viewDir, reflectDir), 0.f), 100.f);
    vec3 specular = vec3(texture(specTex, te_texcoord).r) * spec * sunColor;

    //daytime
    if(te_position.x > 0.f)
    {
        float diff = max(dot(normal, lightDir), 0.f);
        vec3 diffuse = diff * texture(diffuseTex, te_texcoord).rgb;

        float shadow = ShadowCalculation(te_fragPosLightSpace, normal);

        fs_color = vec4(ambient + ((1.f - shadow) * (diffuse + specular)), 1.f);
    }
    else //nighttime
    {
        vec3 lights = max(dot(normal, -lightDir), 0.f) * texture(lightsTex, te_texcoord).rgb;

        fs_color = vec4(ambient + lights, 1.f);
    }

    //fs_color = vec4(normal, 1.f);
}
