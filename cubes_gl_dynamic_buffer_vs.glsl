#version 430

layout (location = 0) uniform mat4 ViewProjection;
layout (std140, binding = 0) uniform CB0
{
    mat4 World;
};

layout (location = 0) in vec3 Attr_Pos;
layout (location = 1) in vec3 Attr_Color;

out vec3 VS_Color;

void main()
{
	vec3 world_pos = vec3(World * vec4(Attr_Pos, 1));
    gl_Position = ViewProjection * vec4(world_pos, 1);
	VS_Color = Attr_Color;
}
