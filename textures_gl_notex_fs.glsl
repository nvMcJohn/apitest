#version 430

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_uniform_buffer_object : require
#extension GL_EXT_gpu_shader4 : require
#extension GL_ARB_shading_language_include : require
#extension GL_ARB_shader_storage_buffer_object : require

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

void main()
{
	int drawID = int(In.iDrawID);
	if (drawID % 2 == 0) {
		Out.v4Color = vec4(1, 0, 0, 1);
	} else {
		Out.v4Color = vec4(0, 0, 1, 1);
	}
}
