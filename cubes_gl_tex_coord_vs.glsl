#version 430

uniform mat4 ViewProjection;

layout (location = 0) in vec3 Attr_Pos;
layout (location = 1) in vec3 Attr_Color;
layout (location = 2) in vec4 Attr_TC0;
layout (location = 3) in vec4 Attr_TC1;
layout (location = 4) in vec4 Attr_TC2;
layout (location = 5) in vec4 Attr_TC3;

out vec3 VS_Color;

void main()
{
    mat4 World = mat4(Attr_TC0, Attr_TC1, Attr_TC2, Attr_TC3);
	vec3 world_pos = vec3(World * vec4(Attr_Pos, 1));
    gl_Position = ViewProjection * vec4(world_pos, 1);
	VS_Color = Attr_Color;
}
