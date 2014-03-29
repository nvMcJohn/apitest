#version 410

// Uniforms / SSBO ----------------------------------------------------------------------------------------------------
uniform mat4 ViewProjection;

// Input --------------------------------------------------------------------------------------------------------------
layout (location = 0) in vec3 In_v3Pos;
layout (location = 1) in vec3 In_v3Color;
layout (location = 2) in vec4 In_m4WorldR0;
layout (location = 3) in vec4 In_m4WorldR1;
layout (location = 4) in vec4 In_m4WorldR2;
layout (location = 5) in vec4 In_m4WorldR3;

// Output -------------------------------------------------------------------------------------------------------------
out block {
    vec3 v3Color;
} Out;

// Functions ----------------------------------------------------------------------------------------------------------
void main()
{
    mat4 World = mat4(In_m4WorldR0, In_m4WorldR1, In_m4WorldR2, In_m4WorldR3);
    vec3 worldPos = vec3(World * vec4(In_v3Pos, 1));
    gl_Position = ViewProjection * vec4(worldPos, 1);
    Out.v3Color = In_v3Color;
}
