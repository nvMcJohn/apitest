#version 430

in vec3 VS_Color;

layout (location = 0) out vec4 FS_Color;

void main()
{
	FS_Color = vec4(VS_Color, 1);
}
