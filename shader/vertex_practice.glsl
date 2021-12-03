#version 330 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Color;
out vec3 out_Color;
uniform mat4 model_transform;
uniform mat4 view_transform;
uniform mat4 proj_transform;

void main( )
{
	gl_Position = proj_transform * view_transform * model_transform * vec4( in_Position, 1.0 );
	out_Color = in_Color;
}