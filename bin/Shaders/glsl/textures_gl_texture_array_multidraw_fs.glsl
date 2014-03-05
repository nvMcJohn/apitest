#version 420
#extension GL_ARB_shader_storage_buffer_object : require

// Uniforms / SSBO ----------------------------------------------------------------------------------------------------
struct Tex2DAddress
{
    uint Container;
    float Page;
};

layout (std140, binding = 1) readonly buffer CB1
{
    Tex2DAddress texAddress[];
};

uniform sampler2DArray TexContainer[16];

// Input --------------------------------------------------------------------------------------------------------------
in block {
    vec2 v2TexCoord;
    flat int iDrawID;
} In;

// Output -------------------------------------------------------------------------------------------------------------
layout(location=0) out vec4 Out_v4Color;

// Functions ----------------------------------------------------------------------------------------------------------
void main()
{
    int drawID = int(In.iDrawID);
    Tex2DAddress addr = texAddress[drawID];
    vec3 texCoord = vec3(In.v2TexCoord.xy, addr.Page);
    vec4 t = texture(TexContainer[addr.Container], texCoord);
    Out_v4Color = vec4(t.xyz, 1);
}
