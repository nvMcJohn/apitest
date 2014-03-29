#version 410

// Uniforms / SSBO ----------------------------------------------------------------------------------------------------
uniform mat4 ViewProjection;
layout(std140) uniform UB0 {
    uniform mat4 World;
};

// Input --------------------------------------------------------------------------------------------------------------
layout(location = 0) in vec3 In_v3Pos;
layout(location = 1) in vec3 In_v3Color;

// Output -------------------------------------------------------------------------------------------------------------
out block {
    vec3 v3Color;
} Out;

// Functions ----------------------------------------------------------------------------------------------------------
void main()
{
    vec3 worldPos = vec3(World * vec4(In_v3Pos, 1));
    gl_Position = ViewProjection * vec4(worldPos, 1);
    Out.v3Color = In_v3Color;
}
