#version 430 core

in vec4 vs_fs_color;

out vec4 FragColor;

void main()
{
	FragColor = vs_fs_color;
}