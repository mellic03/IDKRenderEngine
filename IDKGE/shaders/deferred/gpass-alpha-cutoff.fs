#version 460 core

#extension GL_GOOGLE_include_directive: require
#include "../include/storage.glsl"
#include "../include/util.glsl"
#include "../include/taa.glsl"
#include "../include/noise.glsl"


layout (location = 0) out vec4 fsout_albedo;
layout (location = 1) out vec3 fsout_normal;
layout (location = 2) out vec4 fsout_pbr;
layout (location = 3) out vec4 fsout_vel;


in vec3 fsin_fragpos;
in vec3 fsin_normal;
in vec3 fsin_tangent;
in vec2 fsin_texcoords;
flat in uint drawID;
in IDK_VelocityData fsin_vdata;

in vec3 TBN_viewpos;
in vec3 TBN_fragpos;
in mat3 TBN;
in mat3 TBNT;


void main()
{
    IDK_Camera cam = IDK_GetCamera();
    vec2 texcoords = fsin_texcoords;
    uint offset = IDK_SSBO_texture_offsets[drawID];

    vec4  albedo = texture(IDK_SSBO_textures[offset+0], texcoords).rgba;
    vec3  normal = texture(IDK_SSBO_textures[offset+1], texcoords).xyz * 2.0 - 1.0;
    vec3  ao_r_m = texture(IDK_SSBO_textures[offset+2], texcoords).rgb;
    float noidea = texture(IDK_SSBO_textures[offset+3], texcoords).r;
    float emissv = texture(IDK_SSBO_textures[offset+4], texcoords).r;
    float ao        = ao_r_m.r;
    float roughness = ao_r_m.g;
    float metallic  = ao_r_m.b;

    // vec3 N = normalize(TBN * normalize(normal)); // normalize(fsin_normal);
        //  N = normalize(mix(N, normalize(fsin_normal), 0.5));

    vec2  uv    = IDK_WorldToUV(fsin_fragpos, cam.P * cam.V).xy;
    ivec2 texel = ivec2(uv * vec2(cam.width, cam.height));
    float noise = IDK_BlueNoiseTexel(texel).r;

    // if (albedo.a < noise)
    // {
    //     discard;
    // }

    // if (albedo.a < 0.8)
    // {
    //     discard;
    // }

    fsout_albedo = vec4(albedo.rgb, 1.0);
    fsout_normal = normalize(TBN * normal);
    fsout_pbr    = vec4(roughness, metallic, ao, emissv);
    fsout_vel    = PackVelocity(fsin_vdata);
}
