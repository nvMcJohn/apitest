#version 410
#extension GL_ARB_shader_storage_buffer_object : require

// Uniforms / SSBO ----------------------------------------------------------------------------------------------------
layout (std140, binding = 0) buffer CB0
{
    mat4 Transforms[];
};

uniform mat4 ViewProjection;
uniform int DrawID;

// Input --------------------------------------------------------------------------------------------------------------
layout(location=0) in vec3 In_v3Pos;
layout(location=1) in vec2 In_v2TexCoord;

// Output -------------------------------------------------------------------------------------------------------------
out block {
    vec2 v2TexCoord;
} Out;

// Functions ----------------------------------------------------------------------------------------------------------
void main()
{
    mat4 World = Transforms[DrawID];
    vec4 worldPos = World * vec4(In_v3Pos, 1);
    gl_Position = ViewProjection * worldPos;
    
    Out.v2TexCoord = In_v2TexCoord;
}
