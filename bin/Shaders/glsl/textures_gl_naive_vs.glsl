#version 410

// Uniforms / SSBO ----------------------------------------------------------------------------------------------------
uniform mat4 ViewProjection;
uniform mat4 World;

// Input --------------------------------------------------------------------------------------------------------------
layout(location = 0) in vec3 In_v3Pos;
layout(location = 1) in vec2 In_v2TexCoord;

// Output -------------------------------------------------------------------------------------------------------------
out block {
    vec2 v2TexCoord;
} Out;

// Functions ----------------------------------------------------------------------------------------------------------
void main()
{
    vec4 worldPos = World * vec4(In_v3Pos, 1);
    gl_Position = ViewProjection * worldPos;

    Out.v2TexCoord = In_v2TexCoord;
}
