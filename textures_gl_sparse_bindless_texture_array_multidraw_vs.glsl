#version 430
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shader_draw_parameters : enable

// Uniforms / SSBO --------------------------------------------------------------------------------
layout (std140, binding = 0) buffer CB0
{
    mat4 Transforms[];
};

layout (location = 0) uniform mat4 ViewProjection;


// Input ------------------------------------------------------------------------------------------
layout(location=0) in struct {
    vec3 v3Pos;
    vec2 v2TexCoord;
} In;

in int gl_DrawIDARB;

//  Output ----------------------------------------------------------------------------------------
layout(location=0) out struct {
    vec2 v2TexCoord;
	int iDrawID;
} Out;

void main()
{
    mat4 World = Transforms[gl_DrawIDARB];
	vec4 worldPos = World * vec4(In.v3Pos, 1);
    gl_Position = ViewProjection * worldPos;
	
	Out.v2TexCoord = In.v2TexCoord;
	Out.iDrawID = gl_DrawIDARB;
}
