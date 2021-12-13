#version 330 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Normal;
layout (location = 2) in vec2 in_texture_coord;

out vec3 out_FragPos;
out vec3 out_Normal;
out vec2 texcoord;

uniform mat4 model_transform;
uniform mat4 view_transform;
uniform mat4 proj_transform;

void main( )
{
	gl_Position = proj_transform * view_transform * model_transform * vec4( in_Position, 1.0 );
	out_FragPos = vec3(model_transform * vec4(in_Position, 1.0));
	out_Normal = mat3(transpose(inverse(model_transform))) * in_Normal;
	texcoord = in_texture_coord;
}