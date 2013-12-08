#version 430
#extension GL_ARB_shader_draw_parameters : enable

layout (location = 0) uniform mat4 ViewProjection;
layout (std140, binding = 0) buffer CB0
{
    mat4 Transforms[];
};

layout (location = 0) in vec3 Attr_Pos;
layout (location = 1) in vec3 Attr_Color;

out vec3 VS_Color;

void main()
{
    mat4 World = Transforms[gl_DrawIDARB];
	vec3 world_pos = vec3(World * vec4(Attr_Pos, 1));
    gl_Position = ViewProjection * vec4(world_pos, 1);
	VS_Color = Attr_Color;
}
