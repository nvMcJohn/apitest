#version 410
#extension GL_ARB_shader_storage_buffer_object : require
#extension GL_ARB_bindless_texture : require
#extension GL_NV_gpu_shader5 : enable

// Uniforms / SSBO ----------------------------------------------------------------------------------------------------
struct Tex2DAddress
{
#ifdef GL_NV_gpu_shader5
    uint64_t Container;
#else
    sampler2DArray Container;
#endif
    float Page;
};

layout (std430, binding = 1) buffer CB1
{
    Tex2DAddress texAddress[];
};

// Input --------------------------------------------------------------------------------------------------------------
in block {
    vec2 v2TexCoord;
    flat int iDrawID;
} In;

// Output -------------------------------------------------------------------------------------------------------------
layout(location=0) out vec4 Out_v4Color;

// Functions ----------------------------------------------------------------------------------------------------------
// This is provided to show how you might provide overrides to make client code easier to work with. 
vec4 Texture(sampler2D addr, vec2 uv)
{
    return texture(addr, uv);
}

vec4 Texture(Tex2DAddress addr, vec2 uv)
{
#ifdef GL_NV_gpu_shader5
    return texture(sampler2DArray(addr.Container), vec3(uv, addr.Page));
#else
    return texture(addr.Container, vec3(uv, addr.Page));
#endif
}

void main()
{
    int drawID = int(In.iDrawID);
    Out_v4Color = vec4(Texture(texAddress[drawID], In.v2TexCoord).xyz,  1);
}
