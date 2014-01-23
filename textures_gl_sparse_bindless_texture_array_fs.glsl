#version 430

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_uniform_buffer_object : require
#extension GL_EXT_gpu_shader4 : require
#extension GL_ARB_shading_language_include : require
#extension GL_ARB_shader_storage_buffer_object : require
#extension GL_NV_gpu_shader5 : require

#define MAX_OBJECTS_PER_DRAW  10000
// uniform sampler2DArray sPhysicalTextures[MAX_PHYSICAL_TEXTURES_PER_DRAW];

struct Tex2DAddress
{
	uint64_t Container;
	float Page;
};

layout(location=0) in struct {
    vec2 v2TexCoord;
	flat int iDrawID;
} In;

layout(location=0) out struct {
    vec4 v4Color;
} Out;

layout (std140, binding = 0) readonly buffer CB0
{
    mat4 Transforms[];
};

layout (std430, binding = 1) readonly buffer CB1
{
	Tex2DAddress texAddress[];
};

vec4 Texture(sampler2D addr, vec2 uv)
{
	return texture(addr, uv);
}

vec4 Texture(Tex2DAddress addr, vec2 uv)
{
	return texture(sampler2DArray(addr.Container), vec3(uv, addr.Page));
}

void main()
{
	int drawID = int(In.iDrawID);
	if (texAddress[drawID].Container == 0) {
		Out.v4Color = vec4(1, 0, 0, 1);
	} else if (texAddress[drawID].Page == 0.0f) {
		Out.v4Color = vec4(0, 1, 0, 1);
	} else {
		Out.v4Color = vec4(0, 0, 1, 1);
	}

	Out.v4Color = vec4(Texture(texAddress[drawID], In.v2TexCoord).xyz,  1);
}
