#version 460 core

#extension GL_GOOGLE_include_directive: require
#extension GL_ARB_bindless_texture: require

layout (location = 0) in vec3 vsin_pos;
layout (location = 1) in vec3 vsin_normal;
layout (location = 2) in vec3 vsin_tangent;
layout (location = 3) in vec2 vsin_texcoords;


out vec3 fsin_fragpos;
flat out int idk_LightID;

#include "../include/SSBO_indirect.glsl"
#include "../include/UBOs.glsl"

void main()
{
    idk_LightID = gl_InstanceID;

    IDK_Camera   camera = IDK_RenderData_GetCamera();
    IDK_Dirlight light  = IDK_RenderData_GetDirlight(idk_LightID);

    fsin_fragpos = vsin_pos;

    gl_Position = camera.PV * vec4(vsin_pos, 1.0);
}