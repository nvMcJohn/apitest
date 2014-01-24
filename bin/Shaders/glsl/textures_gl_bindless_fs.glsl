#version 430

#extension GL_ARB_separate_shader_objects : enable

layout(location=0) in struct {
    vec2 v2TexCoord;
} In;

layout(location=0) out struct {
    vec4 v4Color;
} Out;

layout(location = 128) uniform sampler2D gTex;

void main()
{
	Out.v4Color = vec4(texture(gTex, In.v2TexCoord).xyz,  1);
}
