#version 410
#extension GL_ARB_shader_draw_parameters : require
#extension GL_ARB_shader_storage_buffer_object : require

// Uniforms / SSBO ----------------------------------------------------------------------------------------------------
layout (std140, binding = 0) buffer CB0
{
    mat4 Transforms[];
};

uniform mat4 ViewProjection;

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
    mat4 World = Transforms[gl_DrawIDARB];
    vec3 worldPos = vec3(World * vec4(In_v3Pos, 1));
    gl_Position = ViewProjection * vec4(worldPos, 1);
    Out.v3Color = In_v3Color;
}
