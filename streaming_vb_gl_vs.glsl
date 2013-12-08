#version 430

layout(std140, binding = 0) uniform CB0
{
    vec2 Viewport;
};

layout (location = 0) in vec2 Attr_Pos;

void main()
{
    gl_Position = vec4(Attr_Pos * Viewport + vec2(-1, 1), 0, 1);
}
