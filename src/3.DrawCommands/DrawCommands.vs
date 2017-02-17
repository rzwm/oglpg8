#version 430 core

uniform mat4 model_matrix;
uniform mat4 projection_matrix;

layout (location = 0) in vec4 Position;
layout (location = 1) in vec4 Color;

out vec4 vs_fs_color;

void main()
{
	gl_Position = projection_matrix * ( model_matrix * Position);
	vs_fs_color = Color;
}