#version 430

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_bindless_texture : enable
#extension GL_NV_gpu_shader5 : require

// Uniforms / SSBO --------------------------------------------------------------------------------
layout (std430, binding = 1) readonly buffer CB1
{
	uint64_t texAddress[];
};

layout(location=0) in struct {
    vec2 v2TexCoord;
	flat int iDrawID;
} In;

layout(location=0) out struct {
    vec4 v4Color;
} Out;


void main()
{
	sampler2D smplr = sampler2D(texAddress[In.iDrawID]);
	Out.v4Color = vec4(texture(smplr, In.v2TexCoord).xyz,  1);
}
